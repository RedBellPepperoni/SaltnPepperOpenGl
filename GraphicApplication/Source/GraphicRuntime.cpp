#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "AIStuff/AIManager.h"
#include "Engine/Core/Physics/SoftBody/Cloth.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"
#include "SkinAnimator.h"
#include "Engine/Core/Rendering/Renderer/RenderManager.h"

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
      
     
        
        Entity mainCamera = CreateMainCamera(Vector3(0.0f,0.0f,10.0f),Vector3(10.0f));

        Camera* cam = &mainCamera.GetComponent<Camera>();


        //SpawnSkinnedCharacter(Vector3(0.0f));


        Entity characterEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SkinnedCharacter");
        Transform& transform = characterEntity.GetComponent<Transform>();
        transform.SetPosition(Vector3(0.0f,-4.0f,0.0f));
        transform.SetEularRotation(Vector3(0.0f));
        //Assets\\Models\\dancing_vampire.dae
        SkinnedModelComponent& modelComp = characterEntity.AddComponent<SkinnedModelComponent>("Assets\\Models\\dancing_vampire.dae");

        animation = MakeShared<SkinAnimation>("Assets\\Models\\dancing_vampire.dae", modelComp.m_handle);

        SharedPtr<Material>& mat = modelComp.m_handle->meshes[0]->material;

        mat->SetAlbedoTexture("vanguard");

        Application::GetCurrent().GetRenderManager()->GetRenderer()->animator = MakeShared<Animator>(animation);
        animator = Application::GetCurrent().GetRenderManager()->GetRenderer()->animator;

        animator->UpdateAnimation(Time::DeltaTime());

	}

	void OnUpdate(float deltaTime)
	{
       // animator->UpdateAnimation(deltaTime);
	}

   

private:

    //std::vector<int> availableCameras;
    SharedPtr<SkinnedModel> model;
    SharedPtr<Animator> animator;
    SharedPtr<SkinAnimation> animation;

   
   
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