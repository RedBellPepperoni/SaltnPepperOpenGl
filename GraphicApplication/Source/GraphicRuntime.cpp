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

       
      
        /// Water Spawn
        CreateEntity(LakeModel::WATER, Vector3(-0.1594f, -5.7900f, 5.7429f));

        CreateEntity(LakeModel::PLANTS, Vector3(0.0f, -3.0028f, 11.977f));

        // Fishes
        CreateEntity(LakeModel::FISHSCHOOL, Vector3(-1.6167f, -0.8005f, 8.8242f));
        CreateEntity(LakeModel::SHROOMS, Vector3(-1.6167f, -3.6910f, 11.4818f));



        /// Ground Spawn
        CreateEntity(LakeModel::GROUND, Vector3(-0.138f, -6.274f, 5.625f));
        CreateEntity(LakeModel::FENCE, Vector3(1.0621f, 3.3896f, -4.5743f));

        /// Grass
        CreateEntity(LakeModel::GRASS, Vector3(-3.4316f, 1.8853f, -2.1312f), Vector3(0.0f), Vector3(1.0f));
        /// Treessss
        CreateEntity(LakeModel::TREE, Vector3(2.3324f, 2.8829f, -4.1809f),Vector3(0.0f),Vector3(0.7f));
        CreateEntity(LakeModel::TREE, Vector3(3.7187f, 2.8829f, -5.4071f),Vector3(0.0f),Vector3(1.0f));
        CreateEntity(LakeModel::TREE, Vector3(5.5126f, 2.8829f, -5.0939f),Vector3(0.0f),Vector3(0.6f));
        CreateEntity(LakeModel::TREE, Vector3(3.5353f, 2.8829f, -3.4358f),Vector3(0.0f),Vector3(0.7f));
     

        CreateEntity(LakeModel::TREE, Vector3(-4.5897f, 2.8829f, -3.9681f), Vector3(0.0f), Vector3(0.4f));
        CreateEntity(LakeModel::TREE, Vector3(-3.0532f, 2.8829f, -4.6283f), Vector3(0.0f), Vector3(1.0f));
        CreateEntity(LakeModel::TREE, Vector3(-4.7217f, 2.8829f, -5.5748f), Vector3(0.0f), Vector3(0.6f));
        CreateEntity(LakeModel::TREE, Vector3(-2.5809f, 2.8829f, -6.2767f), Vector3(0.0f), Vector3(0.6f));



        /// Dock/Pier
        CreateEntity(LakeModel::DOCK, Vector3(-0.657f, 2.274f, 2.8f));
        CreateEntity(LakeModel::BOAT, Vector3(1.64f, 1.664f, 7.275f));


        /// Shack Stuff
        CreateEntity(LakeModel::SHACK, Vector3(-1.356f, 5.166f, 0.936f));
        CreateEntity(LakeModel::BED, Vector3(-2.96f, 2.70f, -0.914f));
        CreateEntity(LakeModel::BENCH, Vector3(-2.69f, 2.82f, 1.136f));
        CreateEntity(LakeModel::BENCH, Vector3(-0.57f, 2.82f, -1.113f), Vector3(0.0f,90.0f,0.0f));
        CreateEntity(LakeModel::CROW, Vector3(2.589f, 3.45f, 3.43f), Vector3(0.0f,0.0f,0.0f));
        CreateEntity(LakeModel::CROW, Vector3(2.589f, 3.45f, 3.153f), Vector3(180.0f,38.0f,180.0f));
        CreateEntity(LakeModel::CROW, Vector3(2.589f, 3.45f, 3.677f), Vector3(180.0f, -40.7f, 180.0f));
        CreateEntity(LakeModel::CROW, Vector3(-0.6712f, 6.2794f, 1.1263f), Vector3(180.0f, -40.7f, 180.0f),Vector3(1.5f));




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

        Entity mainCamera = CreateMainCamera(Vector3(-3.0854f, 3.9474f, -0.3572f),Vector3(-5.6068f, -67.3442,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


	}

	void OnUpdate(float deltaTime)
	{
        ComponentView school = GetCurrentScene()->GetEntityManager()->GetComponentsOfType<FishSchool>();

       

        for (Entity fishEntity : school)
        {
            Transform& transform = fishEntity.GetComponent<Transform>();
            Quaternion currTor = transform.GetRotation();

            Quaternion YawRot = glm::angleAxis(rotation * 0.001f, Vector3(0.0f, 1.0f, 0.0f));

            Quaternion rot = YawRot * currTor;

            transform.SetRotation(rot);

        }

	}

  

private:

    const float rotation = 10.0f * DEGtoRAD;
   
   
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