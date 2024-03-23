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

       /* CreateEnemyAI(AI::BehaviorState::Seek,EnemyModel::GOBLIN,Vector3(10.0f,0.0f,10.0f));
        CreateEnemyAI(AI::BehaviorState::Approach,EnemyModel::CAT,Vector3(-10.0f,0.0f,-10.0f));
        CreateEnemyAI(AI::BehaviorState::Pursue,EnemyModel::SPIDER,Vector3(20.0f,0.0f,10.0f));
        CreateEnemyAI(AI::BehaviorState::Evade,EnemyModel::DEER,Vector3(10.0f,0.0f,20.0f));
        CreateEnemyAI(AI::BehaviorState::Flee,EnemyModel::SHEEP,Vector3(20.0f,0.0f,20.0f));*/

       /* WanderPArams paramOne{ 6.0f,2.0f,3.0f };
        WanderPArams paramTwo{ 2.0f,18.0f,5.0f };
        WanderPArams paramThree{ 10.0f,20.0f,6.0f };


       CreateWanderAI(paramOne, EnemyModel::GOBLIN, Vector3(-40.0f, 0.0f, 40.0f));
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