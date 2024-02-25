#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/Cloth.h"

struct Ball
{
    bool ball; 
};

void CreateCloth(const Vector3 position)
{
  

    Entity ballEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BallEntity");
    Transform* balltransform = &ballEntity.GetComponent<Transform>();
    ballEntity.AddComponent<Ball>();
    Vector3 ballPos = Vector3(5.0f, 1.0f, 5.0f);
    balltransform->SetPosition(ballPos);


    SharedPtr<SphereCollider> sphereCollider= MakeShared<SphereCollider>(1.0f);
    PhysicsProperties properties;
    properties.collider = sphereCollider;
    properties.mass = 20.0f;
    properties.friction = 0.8f;
    properties.position = Vector3(ballPos);
    RigidBody3D* bodyRef = ballEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
    bodyRef->SetVelocity(Vector3(5.0f, 0.0f, 0.0f));


    Entity clothEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("ClothEntity");
    Transform& clothtransform = clothEntity.GetComponent<Transform>();
    clothtransform.SetPosition(position);

    ClothComponent& clothComp = clothEntity.AddComponent<ClothComponent>(16);
    clothComp.clothHandle->OnInit(balltransform,sphereCollider);
    clothComp.clothHandle->clothMesh->GetMaterial()->SetAlbedoTexture("grass");

}


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

        CreateCloth(Vector3(0.0f,0.0f,0.0f));
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

        ComponentView ballView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Ball>();
        ComponentView clothView = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<ClothComponent>();

        SharedPtr<Cloth>& cloth = clothView[0].GetComponent<ClothComponent>().clothHandle;

        Transform& ballTransform = ballView[0].GetComponent<Transform>();
        Transform& clothtransform = clothView[0].GetComponent<Transform>();
        cloth->OnUpdate(deltaTime, ballTransform, clothtransform);

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