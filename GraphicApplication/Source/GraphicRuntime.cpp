#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/Cloth.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"





class GraphicRuntime : public Application
{
   

    void OnInit()
    {
       
       
        LoadAllModels();
        LoadAllTextures();

        StartPhysics(true);

        MultiThreader::Init();

        //Camera* camera = CreateSecurityCamera(Vector3(0.0f, 10.0f, 0.0f));
       // SharedPtr<Material> mat = CreateMonitor(Vector3(0.0f, 20.0f, 0.0f));
       // mat->textureMaps.albedoMap = (camera->GetRenderTexture());
       

        Entity mainCamera = CreateMainCamera();

        Camera* cam = &mainCamera.GetComponent<Camera>();

       

        CreateDirectionalLight();
       // CreateParentedEntity();

        Entity parentAsteroid = CreateAsteroidParent();
        
        Entity mainAsteroid = CreateMainAsteroid(Vector3(0.0f));
        Entity mainMining = CreateMiningRig();

        Entity indusAsteroid = CreateIndustrialAsteroid();
        Entity indusMining = CreateIndustrialMiningRig();

        mainMining.SetParent(mainAsteroid);
        indusMining.SetParent(indusAsteroid);

        mainAsteroid.SetParent(parentAsteroid);
        indusAsteroid.SetParent(parentAsteroid);


	}



    

	void OnUpdate(float deltaTime)
	{
       
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