#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/VerletCloth.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"

#include "Simulation.h"

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

        Entity mainCamera = CreateMainCamera(Vector3(-19.37f,5.16f,-5.56f),Vector3(0.3f,-76,0));

        Camera* cam = &mainCamera.GetComponent<Camera>();

        CreatePole();
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


       

        float xForce = Random32::Range.GetRandom(-windForce.x, windForce.x);
        Vector3 randomForce = Vector3(xForce, windForce.y, windForce.z) * (speed / 10.0f);

        float time = sin(timeCounter/10.0f);
        if (timeCounter > 20.0f)
        {
            timeCounter = 0.0f;
        }

       /* if (time > 0.5)
        {
            force = true;
        }*/

        force = true;
        
         ComponentView clothCompView = GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Physics::ClothComponent>();
        for (Entity clothEntity : clothCompView)
        {
            Transform& transform = clothEntity.GetComponent<Transform>();
            SharedPtr<Physics::VerletCloth>& cloth = clothEntity.GetComponent<Physics::ClothComponent>().clothHandle;

            cloth->OnUpdate(deltaTime,transform);
            cloth->SetForce(force ? randomForce : Vector3(0.0f));
           

            if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
            {
                clothEntity.Destroy();
                CreateCloth(Vector3(0.0f));
            }

            
            if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Enter))
            {
                int min = 0 + cloth->GetNumPoints()/4;
                int max = cloth->GetNumPoints() - cloth->GetNumPoints() / 4;

                int randomX = Random32::Range.GetRandom(min, max);
                int randomY = Random32::Range.GetRandom(min, max);

                Vector2Int basePoint = Vector2Int(randomX, randomY);
                Vector2Int baseTop = basePoint + Vector2Int(1,0);
                Vector2Int baseside = basePoint + Vector2Int(0,1);

                cloth->RemoveNode(basePoint);
                cloth->RemoveNode(baseTop);
                cloth->RemoveNode(baseside);
            }

            if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Space))
            {
                cloth->Cut();
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
 //               int x = Random32::Range.GetRandom(8,28);
 //               int y = Random32::Range.GetRandom(8,28);

 //               cloth->Cut(Vector2Int((x,y)));
 //             
 //           }

 //           if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::R))
 //           {
 //               
 //               for (int i = 0; i < 32; i++)
 //               {
 //                   cloth->Cut(Vector2Int(2, i));
 //               }
 //              
 //             

 //           }

 //           

 //       }
 ////     
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
    bool clothCut = false;
    Vector3 windForce = Vector3(8.0f, 0.0f, -10.0f);
   
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