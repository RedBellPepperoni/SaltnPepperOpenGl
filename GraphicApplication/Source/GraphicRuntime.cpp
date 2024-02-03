#include "SaltnPepperEngine.h"
#include "Animator.h"


class GraphicRuntime : public Application
{
	void OnInit()
	{
        // Define all the scenes
        m_sceneManager->EnqueSceneFromFile("//Engine//Scene//TestScene.json");
        m_sceneManager->EnqueSceneFromFile("//Engine//Scene//LightScene.json");

        CreateCamera();


        // Creating a new aniamtor/ animation manager
        animator = MakeShared<Animator>(GetCurrentScene()->GetEntityManager());

        CreateAnimatedEntity();
	}

	void OnUpdate(float deltaTime)
	{
        if (InputSystem::GetInstance().GetKeyDown(Key::G))
        {
            startAnimPlayback = !startAnimPlayback;
        }

        if (startAnimPlayback)
        {
            animator->Update(deltaTime);
        }
           
    }



    void CreateCamera()
    {
        Entity cameraEntity = GetCurrentScene()->CreateEntity("Main Camera");
        Transform& transform = cameraEntity.GetComponent<Transform>();

        transform.SetPosition(Vector3(0.0f, 0.0f, -20.0f));
        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));
        
        Camera& camera = cameraEntity.AddComponent<Camera>();
        CameraController& cameraController = cameraEntity.AddComponent<FlyCameraController>();


        cameraController.SetCamera(&camera);
    }

    void CreateAnimatedEntity()
    {
        Entity animatedEntity = GetCurrentScene()->CreateEntity("Animated");
        Transform& transform = animatedEntity.GetComponent<Transform>();

        transform.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));

        animatedEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
        

        AnimationComponent& animComp = animatedEntity.AddComponent<AnimationComponent>();
        animComp.AddPositionKey(Vector3(10.0f,5.0f,0.0f), 2.0f);
        animComp.AddPositionKey(Vector3(0.0f,0.0f,0.0f), 0.0f);
        animComp.AddPositionKey(Vector3(-10.0f,-5.0f,0.0f), 4.0f);



    }

    private:

        SharedPtr<Animator> animator;
        bool startAnimPlayback = false;
       
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