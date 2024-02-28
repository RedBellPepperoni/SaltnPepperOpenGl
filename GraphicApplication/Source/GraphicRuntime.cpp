#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/VerletCloth.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"

//#include "SecurityCamera.h"



class GraphicRuntime : public Application
{
   

    void OnInit()
    {
       
       
        LoadAllModels();
        LoadAllTextures();

        //StartPhysics(true);

        MultiThreader::Init();

        CreateDirectionalLight();

        Entity mainCamera = CreateMainCamera(Vector3(0.0f),Vector3(0.0f));

        Camera* cam = &mainCamera.GetComponent<Camera>();

        CreateCloth(Vector3(0.0f));

	}

	void OnUpdate(float deltaTime)
	{
        bool force = false;

        if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::H))
        {
            force = true;
        }

        ComponentView clothCompView = GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Physics::ClothComponent>();


        Vector3 randomForce = Vector3(Random32::Range.GetRandom(-100, 100), 0.0f, -200.0f);

        for (Entity clothEntity : clothCompView)
        {
            Transform& transform = clothEntity.GetComponent<Transform>();
            SharedPtr<Physics::VerletCloth>& cloth = clothEntity.GetComponent<Physics::ClothComponent>().clothHandle;

            cloth->OnUpdate(deltaTime,transform);
            cloth->SetForce(force ? randomForce: Vector3(0.0f));
            
        }
      
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