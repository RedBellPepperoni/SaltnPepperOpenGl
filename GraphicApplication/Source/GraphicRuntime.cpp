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
      
        
        CreatePointLight(Vector3(-2.0284f, 3.7613f, -2.0895f),5.0f,1.0f);

        CreatePointLight(Vector3(3.2216f, 1.2797f, 13.7093f), 6.0f, 5.0f,Vector3(1.0f,0.2f,0.2f));

        CreatePointLight(Vector3(4.0946f, 3.7613f, -2.0895f), 5.0f, 1.0f, Vector3(0.0f, 1.0f, 0.2f));


      
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

        /// Poles
        CreateEntity(LakeModel::POLE, Vector3(5.0394f, -4.7350f, 15.8551f), Vector3(89.9f,0.0f,0.0f), Vector3(1.0f,1.0f,0.85f));
        CreateEntity(LakeModel::POLE, Vector3(6.4337f, 1.0961f, -4.3310f), Vector3(89.9f,0.0f,0.0f), Vector3(1.0f,1.0f,0.85f));

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

        CreateEntity(LakeModel::TABLE, Vector3(-1.3920f, 2.9666f, -1.0738f),Vector3(0.0f,89.0f,0.0f), Vector3(1.4f));


        /// Render One
        SharedPtr<Texture> camOneTexture = CreateSecurityCamera(Vector3(-2.5003f, 4.7171f, -2.1473f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -20.5682, 0.0f),true);
        SharedPtr<Texture> camTwoTexture = CreateSecurityCamera(Vector3(4.9996f, 1.3538f, 15.7607f), Vector3(0.0f, 20.2819f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
        SharedPtr<Texture> camThreeTexture = CreateSecurityCamera(Vector3(6.3439f, 7.1660f, -4.2813f), Vector3(180.0f, 57.819f, 180.0f), Vector3(0.0f, 0.0f, 0.0f), true);

       

        CreateTV(camOneTexture,Vector3(-1.2152f, 3.5348f, -1.5055f), Vector3(180.0f, 82.0f, 180.0f),DistortType::CHROMATIC);

        CreateTV(camTwoTexture,Vector3(-1.3338f, 3.5348f, -0.5282f), Vector3(0.0f, 82.62f, 0.0f));

        CreateTV(camThreeTexture,Vector3(-1.2803f, 4.0532f, -0.9660f), Vector3(0.0f, 82.62f, 0.0f), DistortType::CHROMATIC);


        
        cameraRenders.push_back(camOneTexture);
        //cameraRenders.push_back(camTwoTexture);
      
        //matOne->textureMaps.albedoMap = camTwoTexture;
        //matTwo->textureMaps.albedoMap = camTwoTexture;


        Entity mainCamera = CreateMainCamera(Vector3(-3.0854f, 3.9474f, -0.3572f),Vector3(-5.6068f, -67.3442,0.00f));

        //Camera* cam = &mainCamera.GetComponent<Camera>();

        CreateSkinnedCharatcer(Vector3(1.27f,2.6f,0.0f),Vector3(0.01f));

        //idleAnim = Application::GetCurrent().GetAnimationLibrary()->GetResource("AJidle");
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


        ComponentView securityView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<SecurityCamera>();

        for (Entity entityCam : securityView)
        {
            Transform& transform = entityCam.GetComponent<Transform>();
            SecurityCamera& cam = entityCam.GetComponent<SecurityCamera>();

            cam.Update(transform, deltaTime);
        }


        ComponentView animaView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<AnimatorComponent>();

        idle = true;

      

        for (Entity entityAnim : animaView)
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
        }
	}

  

private:

    const float rotation = 10.0f * DEGtoRAD;

    bool idle = true;
   
    std::vector<SharedPtr<Texture>> cameraRenders;
    SharedPtr<SkinnedAnimation> idleAnim;

    SharedPtr<Material> matOne = nullptr;
    SharedPtr<Material> matTwo = nullptr;

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