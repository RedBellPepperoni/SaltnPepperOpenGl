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

        MultiThreader::Init();

     
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       

        CreateDirectionalLight();
      
        
        CreatePointLight(Vector3(-20.33f, 216.81f, 51.57),16.0f,1.0f);

       
        CreateWater(Vector3(0.0f,-5.79f,0.0f));


        //// Camera One {Looking from the Rig Pole to the asetroids}
        //SharedPtr<Texture> camOneTexture = CreateSecurityCamera(Vector3(-1167.13f,-20.78f,537.41f),Vector3(-10.20f,-80.28f,0.0f));
        //SharedPtr<Texture> camTwoTexture = CreateSecurityCamera(Vector3(0.76f,-152.31f,-110.21f),Vector3(-155.70f,-1.28f,179.99f));

        //SharedPtr<Texture> camThreeTexture = CreateSecurityCamera(Vector3(409.75f,-169.07f,357.07f),Vector3(-119.27f,-31.65f,179.99f));
        //SharedPtr<Texture> camFourTexture = CreateSecurityCamera(Vector3(-407.96f, -513.03f, 263.41f),Vector3(-134.73f, 52.50, 180.0f));
        //SharedPtr<Texture> camFiveTexture = CreateSecurityCamera(Vector3(-563.30f, 45.88f, 540.15f),Vector3(-2.52f, 80.84f, 0.0f));


        //ScreenRenders.push_back(camOneTexture);
        //ScreenRenders.push_back(camTwoTexture);
        //ScreenRenders.push_back(camThreeTexture);
        //ScreenRenders.push_back(camFourTexture);
        //ScreenRenders.push_back(camFiveTexture);

        /*cornerScreenLeftMat->m_type = MaterialType::Custom;
        cornerScreenRightMat->m_type = MaterialType::Custom;

        cornerScreenLeftMat->name = "Chromatic";
        cornerScreenRightMat->name = "Chromatic";

        cornerScreenLeftMat->SetMetallicTexture("noise");
        cornerScreenRightMat->SetMetallicTexture("noise");

        cornerScreenRightMat->textureMaps.albedoMap = camOneTexture;
        cornerScreenLeftMat->textureMaps.albedoMap = camThreeTexture;*/

        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


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