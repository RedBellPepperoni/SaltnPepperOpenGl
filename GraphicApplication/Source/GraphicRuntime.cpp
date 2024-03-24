#pragma once
#include "SaltnPepperEngine.h"
//#include "EntitySetup.h"
#include "Animator.h"
#include "AnimationDefinitions.h"
#include "SceneLoading/SceneLoader.h"
#include "SceneLoading/SceneParser.h"
#include <sw/redis++/redis++.h>


using namespace sw::redis;

class GraphicRuntime : public Application
{
   

    void CreateBackgroundAudio()
    {
        Entity audioEntity = GetCurrentScene()->CreateEntity("BG_Audio");
      


        Audio::AudioSource* source = AudioManager::GetInstance().CreateSource(audioEntity);
        SharedPtr<AudioClip> clip = GetAudioLibrary()->GetResource("BGMusic");

        source->SetAudioClip(clip.get());
        source->PlayClip();

    }

    

	void OnInit()
	{
#pragma comment(lib, "Ws2_32.lib")

        parser = MakeShared<SceneParser>();
        loader = MakeShared<SceneLoader>();

       

        parser->ParseScene("Engine\\Scene\\DeadEnergy.json");
        
        std::map<std::string, std::string> modelmap = parser->GetModelList();

        std::map<std::string, std::string> texturMap = parser->GetTextureList();

        // Retrives the parsed object data
        std::vector<ObjectData> objectmap = parser->GetObjectList();

        // Load all the models from the model data list
        for (auto const& data : modelmap)
        {
            loader->LoadModel(data.first, data.second);
        }

        for (auto const& texture : texturMap)
        {
            loader->LoadTexture(texture.first, texture.second);
        }

        // Create objects according to the scene data
        for (ObjectData object : objectmap)
        {
            loader->SpawnObject(object);
        }


        AudioLibrary* library = GetAudioLibrary().get();

        SharedPtr<AudioClip> clip = library->LoadAudio("BGMusic", "Assets\\Audio\\EnchantedFestival.mp3");

        animator = MakeShared<Animator>();
        animator->Init(GetCurrentScene()->GetEntityManager());


        GetPhysicsEngine()->SetGravity(Vector3(0.0f, -27.0f, 0.0f));
        StartPhysics(true);

        Application::GetCurrent().GetAppWindow().SetMouseHidden(true);

        CreateBackgroundAudio();
      

        try {

            ConnectionOptions options;

            options.host = "redis-16166.c309.us-east-2-1.ec2.cloud.redislabs.com";
            options.port = 16166;
            options.password = "e2gy7RRilShyw0yoGcLSCe0ZfoGEtcbW";
            options.user = "default";

            std::string url = "redis://default:e2gy7RRilShyw0yoGcLSCe0ZfoGEtcbW@redis-16166.c309.us-east-2-1.ec2.cloud.redislabs.com:16166/0";
            std::string secondurl = "tcp://127.0.0.1:6379";
            auto redis = Redis(options);
            LOG_CRITICAL(redis.ping());


            //get the Inital Top score
            LOG_CRITICAL("Topscore : {0}", redis.get("TopScore").value());

            // Set TopScore
            redis.set("TopScore", "10");
            LOG_CRITICAL("New Topscore : {0}", redis.get("TopScore").value());

           /* redis.setex("redis", 10, "c++");
            redis.setnx("redis++", "c++");*/
        }
        catch (const Error& err) {
            // other errors
            std::cout << err.what() << std::endl;
        }
        
	}



    

	void OnUpdate(float deltaTime)
	{
        ComponentView PlayerView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<PlayerCharacter>();

        Entity player = PlayerView[0];
        Transform* playerTransform = &player.GetComponent<Transform>();
        PlayerCharacter& playerCharacter = player.GetComponent<PlayerCharacter>();
        playerCharacter.Update(deltaTime);


        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Escape))
        {
            bool mouseHidden = Application::GetCurrent().GetAppWindow().GetMouseHidden();
            Application::GetCurrent().GetAppWindow().SetMouseHidden(!mouseHidden);
        }

        animator->Update(deltaTime);


        if (Application::GetCurrent().GetEditorActive())
        {
            return;
        }

        ComponentView TPSCameraView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<FirstPersonCameraController>();

        Entity controller = TPSCameraView[0];
        Transform& transform = controller.GetComponent<Transform>();
        FirstPersonCameraController& FPScontroller = controller.GetComponent<FirstPersonCameraController>();

        Vector2 mousePosition = Input::InputSystem::GetInstance().GetMousePosition();

        FPScontroller.MouseInput(transform,mousePosition,deltaTime);
        playerCharacter.ProcessKeyboardInput(transform,deltaTime);
        playerCharacter.ProcessMouseInput(transform,deltaTime);
	}

private:

    SharedPtr<Animator> animator = nullptr;

    SharedPtr<SceneParser> parser = nullptr;
    SharedPtr<SceneLoader> loader = nullptr;

};

int main(int argc, char* argv)
{
    // Creating a new Appinstance
    GraphicRuntime* app = new GraphicRuntime();

    //FilePath path = File::GetCurrentPath();

    // Always Initialize the App
    app->Initialize();
    // Running the Application
    app->Run();


    delete app;
}