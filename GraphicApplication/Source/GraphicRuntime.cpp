#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "HunterManager.h"



class GraphicRuntime : public Application
{

    
    void OnInit()
    {
       
       
        LoadAllModels();
        LoadAllTextures();

       
        MultiThreader::Init();

     
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
      

       
        CreateDirectionalLight();
      
     
       

        
        Entity mainCamera = CreateMainCamera(Vector3(19.3f,23.01f,14.20f),Vector3(-75.85,-0.67,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();

        
        mainmanager = MakeUnique<HunterManager>();

        mainmanager->CreateDungeon();
        mainmanager->CreateHunters(64);
        //mainmanager->CreateHunters(1);
        mainmanager->CreateTreasures(250);
       
        mainmanager->LaunchHunters();
      
      //  hunters[0]->SetNewTarget(12, 13);
      ///*  hunters[1]->SetNewTarget(20, 3);
      //  hunters[2]->SetNewTarget(21, 1);
      //  hunters[3]->SetNewTarget(19, 14);
      //  hunters[4]->SetNewTarget(12, 12);*/


       
       

	}

   

	void OnUpdate(float deltaTime)
	{
        mainmanager->Update(deltaTime);
        
	}


private:
    UniquePtr<HunterManager> mainmanager = nullptr;
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