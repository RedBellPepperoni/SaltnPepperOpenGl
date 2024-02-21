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


        Camera* camera = CreateSecurityCamera(Vector3(0.0f, 10.0f, 0.0f));
        SharedPtr<Material> mat = CreateMonitor(Vector3(0.0f, 20.0f, 0.0f));
        mat->textureMaps.albedoMap = (camera->GetRenderTexture());
       


        Entity mainCamera = CreateMainCamera();

        Camera* cam = &mainCamera.GetComponent<Camera>();

       

        CreateDirectionalLight();
        CreateParentedEntity();
        CreateBaseFloor();


        aiManager = MakeShared<AI::AIManager>();


        CreatePlayerCharacter(mainCamera);
      /*  CreateEnemyAI(AI::BehaviorState::Seek,EnemyModel::GOBLIN,Vector3(10.0f,0.0f,10.0f));
        CreateEnemyAI(AI::BehaviorState::Approach,EnemyModel::CAT,Vector3(-10.0f,0.0f,-10.0f));
        CreateEnemyAI(AI::BehaviorState::Pursue,EnemyModel::SPIDER,Vector3(20.0f,0.0f,10.0f));
        CreateEnemyAI(AI::BehaviorState::Evade,EnemyModel::DEER,Vector3(10.0f,0.0f,20.0f));
        CreateEnemyAI(AI::BehaviorState::Flee,EnemyModel::SHEEP,Vector3(20.0f,0.0f,20.0f));*/

       
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