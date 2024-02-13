#include "SaltnPepperEngine.h"
#include "EntitySetup.h"



class GraphicRuntime : public Application
{
	void OnInit()
	{
 
        LoadAllModels();
        LoadAllTextures();

       

        Entity mainCamera = CreateMainCamera();
        CreateDirectionalLight();
        Entity leftHand = CreateHand(true);
        Entity rightHand = CreateHand(false);
        CreatePlayerCharacter(mainCamera, leftHand, rightHand);
        CreateBaseFloor();


      
        CreatePlatform(Vector3(10.0f, 2.0f, 15.0f), Vector3(5.0f, 2.0f, 5.0f));
        CreatePlatform(Vector3(20.0f, 4.0f, 15.0f), Vector3(5.0f, 2.0f, 5.0f));
        CreatePlatform(Vector3(35.0f, 5.0f, 15.0f), Vector3(5.0f, 2.0f, 5.0f));
        CreatePlatform(Vector3(10.0f, 7.0f, 5.0f), Vector3(5.0f, 2.0f, 5.0f));
        //CreatePlatform(Vector3(10.0f, 4.0f, 5.0f), Vector3(5.0f, 2.0f, 5.0f));

        GetPhysicsEngine()->SetGravity(Vector3(0.0f, -27.0f, 0.0f));
        StartPhysics(true);

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
	}

private:

   

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