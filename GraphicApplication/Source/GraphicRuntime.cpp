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

        //Camera* camera = CreateSecurityCamera(Vector3(0.0f, 10.0f, 0.0f));
       // SharedPtr<Material> mat = CreateMonitor(Vector3(0.0f, 20.0f, 0.0f));
       // mat->textureMaps.albedoMap = (camera->GetRenderTexture());
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       

        CreateDirectionalLight();
      
     
        createAsteroidScene();
        createConsoleScene();
       

        CreatePointLight(Vector3(-20.33f, 216.81f, 51.57),16.0f,1.0f);

        //blacktexture = m_textureLibrary->GetResource("Black");
         
        // Camera One {Looking from the Rig Pole to the asetroids}
        SharedPtr<Texture> camOneTexture = CreateSecurityCamera(Vector3(-1167.13f,-20.78f,537.41f),Vector3(-10.20f,-80.28f,0.0f));
        SharedPtr<Texture> camTwoTexture = CreateSecurityCamera(Vector3(0.76f,-152.31f,-110.21f),Vector3(-155.70f,-1.28f,179.99f));

        SharedPtr<Texture> camThreeTexture = CreateSecurityCamera(Vector3(409.75f,-169.07f,357.07f),Vector3(-119.27f,-31.65f,179.99f));
        SharedPtr<Texture> camFourTexture = CreateSecurityCamera(Vector3(-407.96f, -513.03f, 263.41f),Vector3(-134.73f, 52.50, 180.0f));
        SharedPtr<Texture> camFiveTexture = CreateSecurityCamera(Vector3(-563.30f, 45.88f, 540.15f),Vector3(-2.52f, 80.84f, 0.0f));


        ScreenRenders.push_back(camOneTexture);
        ScreenRenders.push_back(camTwoTexture);
        ScreenRenders.push_back(camThreeTexture);
        ScreenRenders.push_back(camFourTexture);
        ScreenRenders.push_back(camFiveTexture);

        cornerScreenLeftMat->m_type = MaterialType::Custom;
        cornerScreenRightMat->m_type = MaterialType::Custom;

        cornerScreenLeftMat->name = "Chromatic";
        cornerScreenRightMat->name = "Chromatic";

        cornerScreenLeftMat->SetMetallicTexture("noise");
        cornerScreenRightMat->SetMetallicTexture("noise");

        cornerScreenRightMat->textureMaps.albedoMap = camOneTexture;
        cornerScreenLeftMat->textureMaps.albedoMap = camThreeTexture;

        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


	}

	void OnUpdate(float deltaTime)
	{
        if (Input::InputSystem::GetInstance().GetKeyDown(Key::num2) || Input::InputSystem::GetInstance().GetKeyDown(Key::Numpad2))
        {
            ToggleScreenActive();
        }

        ApplyCameraCycle(deltaTime);


        ComponentView securityView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<SecurityCamera>();

        for (Entity entityCam : securityView)
        {
            Transform& transform = entityCam.GetComponent<Transform>();
            SecurityCamera& cam = entityCam.GetComponent<SecurityCamera>(); 

            cam.Update(transform, deltaTime);
        }

        ComponentView screenview = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<ScifiScreen>();

        for (Entity entityScreen : screenview)
        {
            ScifiScreen& screen = entityScreen.GetComponent<ScifiScreen>();

            screen.Update(deltaTime);
        }
	}

    void ApplyCameraCycle(float deltaTime)
    {

        if (screenOff) 
        {
            leftScreenCounter = leftScreenSwitchTime +1;
            rightScreenCounter = rightScreenSwitchTime +1;
            return; 
        }

        

        //Left Screnn Cycle Logic
        leftScreenCounter += deltaTime;
        if (leftScreenCounter > leftScreenSwitchTime) 
        { 
            cornerScreenLeftMat->textureMaps.albedoMap = CycleCameraTexture(false); 
            leftScreenCounter = 0.0f;
        }

        // Right Screen Cycle Logic
        rightScreenCounter += deltaTime;
        if (rightScreenCounter > rightScreenSwitchTime) 
        { 
            cornerScreenRightMat->textureMaps.albedoMap = CycleCameraTexture(true); 
            rightScreenCounter = 0.0f;
        }


    }

    SharedPtr<Texture>& CycleCameraTexture(bool isRightScreen)
    {
        if (isRightScreen)
        {   
            rightScreenIndex++;
            if (rightScreenIndex >= ScreenRenders.size()) {rightScreenIndex = 0;}

            return ScreenRenders[rightScreenIndex];
        }

        leftScreenIndex++;
        if (leftScreenIndex >= ScreenRenders.size()) { leftScreenIndex = 0; }

        return ScreenRenders[leftScreenIndex];
           
        
    }

    void createAsteroidScene()
    {
        // ======== Asteroid Entities ===================
     Entity parentAsteroid = CreateAsteroidParent();

     Entity mainAsteroid = CreateMainAsteroid(Vector3(0.0f));
     Entity mainMining = CreateMiningRig();

     Entity indusAsteroid = CreateIndustrialAsteroid();
     Entity indusMining = CreateIndustrialMiningRig();

     Entity otherAsteroid = CreateSecondaryAsteroid();
     Entity otherMining = CreateSecondaryMiningRig();

     mainMining.SetParent(mainAsteroid);
     indusMining.SetParent(indusAsteroid);
     otherMining.SetParent(otherAsteroid);

     mainAsteroid.SetParent(parentAsteroid);
     indusAsteroid.SetParent(parentAsteroid);
     otherAsteroid.SetParent(parentAsteroid);
    }

    void createConsoleScene()
    {
        Entity consoleRoom = CreateConsoleRoom(Vector3(-20.07f, 214.89f, 49.24f));
        Entity centerConsole = CreateCenterConsole(Vector3(2.5f, 0.0f, 0.0f));
        Entity cornerConsoleRight = CreateCornerConsoleRight(Vector3(7.5f, 0.0f, 5.0f));
        Entity cornerConsoleLeft = CreateCornerConsoleLeft(Vector3(-7.5f, 0.0f, 5.0f));


        centerConsole.SetParent(consoleRoom);
        cornerConsoleRight.SetParent(consoleRoom);
        cornerConsoleLeft.SetParent(consoleRoom);

        Entity centerWindow = CreateCenterWindow(Vector3(0.0f));
        centerWindow.SetParent(centerConsole);

        Entity cornerwinRight = CreateCornerWindow(Vector3(0.0f));
        cornerwinRight.SetParent(cornerConsoleRight);

        Entity cornerwinLeft = CreateCornerWindow(Vector3(0.0f), true);
        cornerwinLeft.SetParent(cornerConsoleLeft);

        cornerScreenRightMat = CreateCornerScreen(cornerConsoleRight, Vector3(0.0f), true);
        cornerScreenLeftMat = CreateCornerScreen(cornerConsoleLeft, Vector3(0.0f), false);

        Entity centerScreen_Left = CreateCenterScreen(Vector3(0.0f), false);
        Entity centerScreen_Right = CreateCenterScreen(Vector3(0.0f), true);

        centerScreen_Left.SetParent(centerConsole);
        centerScreen_Right.SetParent(centerConsole);

        CreateFloorCeiling(consoleRoom, Vector3(0.0f, 0.0f, 0.0f));
        CreateFloorCeiling(consoleRoom, Vector3(5.0f,0.0f,0.0f));
        CreateFloorCeiling(consoleRoom, Vector3(5.0f, 0.0f, 5.0f));
        CreateFloorCeiling(consoleRoom, Vector3(0.0f, 0.0f, 5.0f));
    }
   
    void ToggleScreenActive()
    {
        screenOff = !screenOff;


        ComponentView screenview = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<ScifiScreen>();

        for (Entity entityScreen : screenview)
        {
            ScifiScreen& screen = entityScreen.GetComponent<ScifiScreen>();

            screen.SwitchActive(!screenOff);
        }

        if (screenOff)
        {
            cornerScreenRightMat->SetAlbedoTexture("Black");
            cornerScreenLeftMat->SetAlbedoTexture("Black");
        }

        
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