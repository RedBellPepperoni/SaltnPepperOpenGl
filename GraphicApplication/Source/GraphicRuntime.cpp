#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "AIStuff/BoidManager.h"


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
        boidManager = MakeUnique<AI::BoidManager>();

        boidManager->OnInit();

        boidManager->CreatePredator(Vector3(15.0f, 0.0f, 25.0f));

        // Create Flock
        boidManager->CreateFlockEntity(Vector3(10.0f,0.0f,5.0f));
        boidManager->CreateFlockEntity(Vector3(15.0f,0.0f,5.0f));
        boidManager->CreateFlockEntity(Vector3(20.0f,0.0f,5.0f));
        boidManager->CreateFlockEntity(Vector3(10.0f,0.0f,15.0f));
        boidManager->CreateFlockEntity(Vector3(15.0f,0.0f,15.0f));
        boidManager->CreateFlockEntity(Vector3(15.0f,0.0f,25.0f));
        boidManager->CreateFlockEntity(Vector3(15.0f,0.0f,20.0f));


        boidManager->CreateFlockEntity(Vector3(40.0f, 0.0f, 5.0f));
        boidManager->CreateFlockEntity(Vector3(35.0f, 0.0f, 5.0f));
        boidManager->CreateFlockEntity(Vector3(35.0f, 0.0f, 5.0f));
        boidManager->CreateFlockEntity(Vector3(30.0f, 0.0f, 15.0f));
        boidManager->CreateFlockEntity(Vector3(35.0f, 0.0f, 15.0f));
        boidManager->CreateFlockEntity(Vector3(30.0f, 0.0f, 20.0f));


        boidManager->CreateFlockEntity(Vector3(-30.0f, 0.0f, -30.0f));
        boidManager->CreateFlockEntity(Vector3(-35.0f, 0.0f, -30.0f));
        boidManager->CreateFlockEntity(Vector3(-40.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(-45.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(-45.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(-50.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(-50.0f, 0.0f, -20.0f));


        boidManager->CreateFlockEntity(Vector3(30.0f, 0.0f, -30.0f));
        boidManager->CreateFlockEntity(Vector3(35.0f, 0.0f, -30.0f));
        boidManager->CreateFlockEntity(Vector3(40.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(45.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(45.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(50.0f, 0.0f, -25.0f));
        boidManager->CreateFlockEntity(Vector3(50.0f, 0.0f, -20.0f));

      // Create Obstacles
        boidManager->CreateObstacle(Vector3(0.0f, 0.0f, 0.0f),true);
        boidManager->CreateObstacle(Vector3(25.0f, 0.0f, 5.0f));
        boidManager->CreateObstacle(Vector3(-20.0f, 0.0f, 5.0f));
      /*  boidManager->CreateObstacle(Vector3(10.0f, 2.0f, 15.0f));
        boidManager->CreateObstacle(Vector3(15.0f, 2.0f, 15.0f));
        boidManager->CreateObstacle(Vector3(15.0f, 2.0f, 25.0f));
        boidManager->CreateObstacle(Vector3(15.0f, 2.0f, 20.0f));*/


        CreatePlayerCharacter(mainCamera);

     
        CreateBreadScene();


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

        if (boidManager)
        {
            boidManager->OnUpdate(deltaTime);
        }


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


    void CreateBreadScene()
    {
        CreateBread(Vector3(0.0367f, 0.0f, 0.5524f), Vector3(0.0f, 50.4761f, 0.0f), Vector3(7.0f));
        CreateBread(Vector3(0.6480f, 0.0f, 0.2416f), Vector3(0.0f, -42.6199f, 0.0f), Vector3(7.0f));
        CreateBread(Vector3(-0.5872f, 0.0f, 0.3864f), Vector3(180.0f, 89.4249f, 180.0f), Vector3(7.0f));
        CreateBread(Vector3(-0.6210f, 0.0f, -0.1543f), Vector3(180.0f, -46.8466f,180.0f), Vector3(7.0f));
        CreateBread(Vector3(-0.1560f, 0.0f, -0.5662f), Vector3(180.0f, -69.2301f, 180.0f), Vector3(7.0f));
        CreateBread(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(7.0f));
        CreateBread(Vector3(0.4959f, 0.0f, -0.4508f), Vector3(0.0f, 48.6778f, 0.0f), Vector3(8.0f));
  
        CreateBread(Vector3(-0.2858f, 0.2534f, -0.1988f), Vector3(0.0f, 0.0f, 0.0f), Vector3(7.0f));
        CreateBread(Vector3(0.4040f, 0.2534f, 0.1299f), Vector3(0.0f, -80.0934f, 0.0f), Vector3(7.0f));
    
    }


private:

    SharedPtr<AI::AIManager> aiManager = nullptr;
    UniquePtr<AI::BoidManager> boidManager = nullptr;

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