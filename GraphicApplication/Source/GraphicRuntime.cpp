#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"


class GraphicRuntime : public Application
{
	void OnInit()
	{
        //Define all the scenes
        //m_sceneManager->EnqueSceneFromFile("//Engine//Scene//TestScene.json");
        //m_sceneManager->EnqueSceneFromFile("//Engine//Scene//LightScene.json");
        //m_sceneManager->NextScene();
        //m_sceneManager->SwitchScene("TestScene");

        LoadAllModels();
        LoadAllTextures();

        StartPhysics(true);

        Entity mainCamera = CreateMainCamera();
        CreateDirectionalLight();
        CreateParentedEntity();
        CreateBaseFloor();


        aiManager = MakeShared<AI::AIManager>();


        CreatePlayerCharacter(mainCamera);
      
        Vector3 centerOne = Vector3{ -39.68f,0.0f,8.33f };
        std::vector<Vector3> pathOne
        {
            Vector3{-50.73f,0.0f,21.56f},
            Vector3{-24.94f,0.0f,17.61f},
            Vector3{-29.74f,0.0f,-3.02f},
            Vector3{-51.37f,0.0f,-3.0f},
        };

        Vector3 centerTwo = Vector3{ -18.13f,0.0f,22.09f };
        std::vector<Vector3> pathTwo
        {
        
           Vector3{-27.70f,0.0f,4.14f},
            Vector3{-0.20f,0.0f,13.29f},
            Vector3{-8.51f,0.0f,47.34f},
            Vector3{-27.79f,0.0f,30.98f},
        };


        CreatePathFollowAI(pathOne,centerOne,EnemyModel::CAT, Vector3{ -50.73f,0.0f,21.56f });
        CreatePathFollowAI(pathTwo,centerTwo,EnemyModel::GOBLIN, Vector3{ -27.70f,0.0f,4.14f });

      /* CreateWanderAI(paramOne, EnemyModel::GOBLIN, Vector3(-40.0f, 0.0f, 40.0f));
       CreateWanderAI(paramTwo, EnemyModel::SHEEP, Vector3(-35.0f, 0.0f,40.0f));
       CreateWanderAI(paramThree, EnemyModel::CAT, Vector3(-40.0f, 0.0f, 35.0f));*/
       

       

	}



    

	void OnUpdate(float deltaTime)
	{
        ComponentView PlayerView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<PlayerCharacter>();

        Entity player = PlayerView[0];
        Transform* playerTransform = &player.GetComponent<Transform>();
        PlayerCharacter& playerCharacter = player.GetComponent<PlayerCharacter>();
        playerCharacter.Update(deltaTime);

        ComponentView PlayerLookView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<PlayerLook>();

        Transform* lookTransform = &PlayerLookView[0].GetComponent<Transform>();

        aiManager->Update(deltaTime, playerTransform, lookTransform);

        if (Application::GetCurrent().GetEditorActive())
        {
            return;
        }

        ComponentView TPSCameraView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<ThirdPersonCameraController>();

        Entity controller = TPSCameraView[0];
        Transform& transform = controller.GetComponent<Transform>();
        ThirdPersonCameraController& TPScontroller = controller.GetComponent<ThirdPersonCameraController>();

        Vector2 mousePosition = Input::InputSystem::GetInstance().GetMousePosition();

        TPScontroller.MouseInput(transform,mousePosition,deltaTime);
        playerCharacter.ProcessKeyboardInput(transform,deltaTime);
	}

private:

    SharedPtr<AI::AIManager> aiManager = nullptr;

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