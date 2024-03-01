#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "Engine/Core/Threading/MultiThreader.h"




class GraphicRuntime : public Application
{
   

    void OnInit()
    {
       
       
        LoadAllModels();
        LoadAllTextures();

        //StartPhysics(true);

        MultiThreader::Init();

        //Camera* camera = CreateSecurityCamera(Vector3(0.0f, 10.0f, 0.0f));
       // SharedPtr<Material> mat = CreateMonitor(Vector3(0.0f, 20.0f, 0.0f));
       // mat->textureMaps.albedoMap = (camera->GetRenderTexture());
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       

        CreateDirectionalLight();
      
     
       

        
        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


	}

	void OnUpdate(float deltaTime)
	{
       
	}

   
   
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