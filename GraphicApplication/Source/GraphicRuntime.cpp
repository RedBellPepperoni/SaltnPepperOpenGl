#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/Cloth.h"
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

       
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       

        CreateDirectionalLight();
      
     
        
        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


        SpawnSkinnedCharacter(Vector3(0.0f));

	}

	void OnUpdate(float deltaTime)
	{
        
	}

   

private:

    //std::vector<int> availableCameras;
    int rightScreenIndex = 0;
    int leftScreenIndex = 2;

    float leftScreenCounter = 0.0f;
    float rightScreenCounter = 0.2f;

    const float leftScreenSwitchTime = 5.0f;
    const float rightScreenSwitchTime = 4.0f;

    std::vector<SharedPtr<Texture>> ScreenRenders;
    SharedPtr<Material> cornerScreenRightMat = nullptr;
    SharedPtr<Material> cornerScreenLeftMat = nullptr;

    bool screenOff = false;


   
   
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