#include "SaltnPepperEngine.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Threading/MultiThreader.h"

#include "GameManager.h"
//#include "SecurityCamera.h"



class GraphicRuntime : public Application
{
   

    void OnInit()
    {
       
        gameManager = MakeUnique<GameManager>();
        gameManager->OnInit();

      /*  /// Render One
        SharedPtr<Texture> camOneTexture = CreateSecurityCamera(Vector3(-2.5003f, 4.7171f, -2.1473f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -20.5682, 0.0f),true);
        SharedPtr<Texture> camTwoTexture = CreateSecurityCamera(Vector3(4.9996f, 1.3538f, 15.7607f), Vector3(0.0f, 20.2819f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
        SharedPtr<Texture> camThreeTexture = CreateSecurityCamera(Vector3(6.3439f, 7.1660f, -4.2813f), Vector3(180.0f, 57.819f, 180.0f), Vector3(0.0f, 0.0f, 0.0f), true);

       

        CreateTV(camOneTexture,Vector3(-1.2152f, 3.5348f, -1.5055f), Vector3(180.0f, 82.0f, 180.0f),DistortType::CHROMATIC);

        CreateTV(camTwoTexture,Vector3(-1.3338f, 3.5348f, -0.5282f), Vector3(0.0f, 82.62f, 0.0f));

        CreateTV(camThreeTexture,Vector3(-1.2803f, 4.0532f, -0.9660f), Vector3(0.0f, 82.62f, 0.0f), DistortType::CHROMATIC);
*/

        
       // cameraRenders.push_back(camOneTexture);
        //cameraRenders.push_back(camTwoTexture);
      
        //matOne->textureMaps.albedoMap = camTwoTexture;
        //matTwo->textureMaps.albedoMap = camTwoTexture;


        //Entity mainCamera = EntitySetup::CreateMainCamera(Vector3(-3.0854f, 3.9474f, -0.3572f),Vector3(-5.6068f, -67.3442,0.00f));
        //Camera* cam = &mainCamera.GetComponent<Camera>();
        //idleAnim = Application::GetCurrent().GetAnimationLibrary()->GetResource("AJidle");
	}

	void OnUpdate(float deltaTime)
	{
       
        gameManager->OnUpdate(deltaTime);


       /* ComponentView animaView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<AnimatorComponent>();

        idle = true;

      */

      /*  for (Entity entityAnim : animaView)
        {
            AnimatorComponent& comp = entityAnim.GetComponent<AnimatorComponent>();


            if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Up))
            {
                idle = false;
                comp.GetAnimator()->PlayAnimationbyName("Walk");
            }

            if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Left))
            {
                idle = false;
                comp.GetAnimator()->PlayAnimationbyName("StrafeLeft");
            }

            if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Right))
            {
                idle = false;
                comp.GetAnimator()->PlayAnimationbyName("StrafeRight");
            }

            if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Down))
            {
                idle = false;
                comp.GetAnimator()->PlayAnimationbyName("Dance");
            }

            if (idle == true)
            { 
                comp.GetAnimator()->PlayAnimationbyName("Idle");
            }



            comp.GetAnimator()->OnUpdate(deltaTime);
        }*/

      
	}

  

private:

   
    bool idle = true;

    UniquePtr<GameManager> gameManager = nullptr;
   
  /*  std::vector<SharedPtr<Texture>> cameraRenders;
    SharedPtr<SkinnedAnimation> idleAnim;

    SharedPtr<Material> matOne = nullptr;
    SharedPtr<Material> matTwo = nullptr;*/

  

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