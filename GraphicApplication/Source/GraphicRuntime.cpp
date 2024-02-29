#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/VerletCloth.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"

//#include "Simulation.h"

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

        if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::NumpadAdd))
        {
            IncreaseForce(true);
        }
        else if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::NumpadSubtract))
        {
            IncreaseForce(false);
        }

      

        timeCounter += deltaTime * 5;


        ComponentView clothCompView = GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Physics::ClothComponent>();

        float xForce = Random32::Range.GetRandom(-windForce.x, windForce.x);
        Vector3 randomForce = Vector3(xForce, windForce.y, windForce.z) * (speed / 10.0f);

        float time = sin(timeCounter);
        if (timeCounter > 50.0f)
        {
            timeCounter = 0.0f;
        }

        if (time < 0.5)
        {
            force = true;
        }
        

        for (Entity clothEntity : clothCompView)
        {
            Transform& transform = clothEntity.GetComponent<Transform>();
            SharedPtr<Physics::VerletCloth>& cloth = clothEntity.GetComponent<Physics::ClothComponent>().clothHandle;

            cloth->OnUpdate(deltaTime,transform);
            cloth->SetForce(force ? randomForce : Vector3(0.0f));
            
            if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Enter))
            {
                cloth->RemoveNode(Vector2Int(8,8));
                cloth->RemoveNode(Vector2Int(9,9));
                cloth->RemoveNode(Vector2Int(3,25));
                cloth->RemoveNode(Vector2Int(30,8));
            }

        }


 //       ComponentView clothCompView = GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<VerletClothComponent>();
 //       for (Entity clothEntity : clothCompView)
 //       {
 //           //Transform& transform = clothEntity.GetComponent<Transform>();
 //           SharedPtr<Simulation>& cloth = clothEntity.GetComponent<VerletClothComponent>().clothsim;

 //           cloth->OnUpdate(deltaTime);

 //           if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Enter))
 //           {    
 //               cloth->Cut(Vector2Int(8,8));
 //             
 //           }

 //           if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::R))
 //           {
 //               cloth->Cut(Vector2Int(16, 16));

 //           }

 //       }
 //     
	}

   
private:

    void IncreaseForce(bool increase)
    {
        increase? speed++ : speed--;
        
        Clamp(speed, minSpeed, maxSpeed);
    }

    float timeCounter = 0.0f;
    int speed = 1;
    const int maxSpeed = 10;
    const int minSpeed = 0;
    Vector3 windForce = Vector3(30.0f, 3.0f, -40.0f);
   
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