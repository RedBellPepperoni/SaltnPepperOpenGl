#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "DungeonManager.h"



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
      

        manager = MakeShared<DungeonManager>();

        manager->LoadTSVMap("Assets\\Data\\DungeonCleanCompact.tsv");
        manager->SetStartandEnd();

        manager->PrintMap();

        CreateDirectionalLight();
      
     
       

        
        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


	}

	void OnUpdate(float deltaTime)
	{
       
	}

    SharedPtr<DungeonManager> manager;
   
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