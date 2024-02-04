#include "SaltnPepperEngine.h"
#include "Animator.h"
#include "ColorChangeScript.h"


class GraphicRuntime : public Application
{
	void OnInit()
	{
        // Define all the scenes
        m_sceneManager->EnqueSceneFromFile("\\Engine\\Scene\\TestScene.json");
        m_sceneManager->EnqueSceneFromFile("\\Engine\\Scene\\LightScene.json");

        GetModelLibrary()->LoadModel("GyroOuter", "Assets\\Models\\Gyro_Outer.fbx");
        GetModelLibrary()->LoadModel("GyroMiddle", "Assets\\Models\\Gyro_Middle.fbx");
        GetModelLibrary()->LoadModel("GyroInner", "Assets\\Models\\Gyro_Inner.fbx");
        GetModelLibrary()->LoadModel("LegoSpaceman", "Assets\\Models\\LegoSpaceman.fbx");
        GetModelLibrary()->LoadModel("LegoSkeleton", "Assets\\Models\\LegoSkeleton.fbx");

        CreateCamera();
        CreateDirectionalLight();

        // Creating a new aniamtor/ animation manager
        animator = MakeShared<Animator>(GetCurrentScene()->GetEntityManager());

        CreateAnimatedCubeRotation(EasingType::Linear);
        CreateAnimatedCubeRotation(EasingType::SineEaseIn);
        CreateAnimatedCubeRotation(EasingType::SineEaseOut);
        CreateAnimatedCubeRotation(EasingType::SineEaseInOut);

        CreateAnimatedCubeScale(EasingType::Linear);
        CreateAnimatedCubeScale(EasingType::SineEaseIn);
        CreateAnimatedCubeScale(EasingType::SineEaseOut);
        CreateAnimatedCubeScale(EasingType::SineEaseInOut);

        CreateAnimatedCubePosition(EasingType::Linear);
        CreateAnimatedCubePosition(EasingType::SineEaseIn);
        CreateAnimatedCubePosition(EasingType::SineEaseOut);
        CreateAnimatedCubePosition(EasingType::SineEaseInOut);

        CreateAnimatedCubeEvent();
	}

	void OnUpdate(float deltaTime)
	{
        if (InputSystem::GetInstance().GetKeyDown(Key::Space))
        {
            startAnimPlayback = !startAnimPlayback;
        }

        if (InputSystem::GetInstance().GetKeyDown(Key::num1))
        {
            animator->SetAnimSpeed(1.0f);
        }
        else if (InputSystem::GetInstance().GetKeyDown(Key::num2))
        {
            animator->SetAnimSpeed(2.0f);
        }
        else if (InputSystem::GetInstance().GetKeyDown(Key::num3))
        {
            animator->SetAnimSpeed(3.0f);
        }
        else if (InputSystem::GetInstance().GetKeyDown(Key::num4))
        {
            animator->SetAnimSpeed(4.0f);
        }
        else if (InputSystem::GetInstance().GetKeyDown(Key::num5))
        {
            animator->SetAnimSpeed(5.0f);
        }

        if (startAnimPlayback)
        {
            animator->Update(deltaTime);
        }
           
    }



    void CreateCamera()
    {
        Entity cameraEntity = GetCurrentScene()->CreateEntity("Main Camera");
        Transform& transform = cameraEntity.GetComponent<Transform>();

        transform.SetPosition(Vector3(10.7f, 2.8f, 45.9f));
        transform.SetEularRotation(Vector3(-3.31f, -2.89f, 0.0f));
        
        Camera& camera = cameraEntity.AddComponent<Camera>();
        CameraController& cameraController = cameraEntity.AddComponent<FlyCameraController>();


        cameraController.SetCamera(&camera);
    }

    void CreateDirectionalLight()
    {
        Entity dirlightEntity = GetCurrentScene()->CreateEntity("Main Camera");
        Transform& transform = dirlightEntity.GetComponent<Transform>();

        transform.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetEularRotation(Vector3(-10.0f, 30.0f, 0.0f));

        Light& light = dirlightEntity.AddComponent<Light>();
        light.intensity = 1.0f;
        light.type = LightType::DirectionLight;
    }

    Entity CreateAnimatedCubeRotation(EasingType type)
    {
        Entity animatedEntity = GetCurrentScene()->CreateEntity("RotationCube");
        Transform& transform = animatedEntity.GetComponent<Transform>();

        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetScale(Vector3(2.0f));

        SharedPtr<Model>& model = animatedEntity.AddComponent<ModelComponent>(PrimitiveType::Cube).m_handle;
        const SharedPtr<Material>& material = model->GetMeshes()[0]->GetMaterial();

       
        AnimationComponent& animComp = animatedEntity.AddComponent<AnimationComponent>();
   
       


        switch (type)
        {
        case EasingType::Linear:  transform.SetPosition(Vector3(-10.0f, 0.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case EasingType::SineEaseIn:  transform.SetPosition(Vector3(-10.0f, 10.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case EasingType::SineEaseOut:  transform.SetPosition(Vector3(-10.0f, 20.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case EasingType::SineEaseInOut:  transform.SetPosition(Vector3(-10.0f, -10.0f, 0.0f));
            material->albedoColour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        default:
            break;
        }


        animComp.AddRotationKey(Vector3(0.0f), 0.0f);
        animComp.AddRotationKey(Vector3(0.0f,90.0f,0.0f), 3.0f,type);
        animComp.AddRotationKey(Vector3(0.0f,0.0f,90.0f), 6.0f, type);


        animComp.loop = true;


        return animatedEntity;
    }

    Entity CreateAnimatedCubeScale(EasingType type)
    {
        Entity animatedEntity = GetCurrentScene()->CreateEntity("ScaleCube");
        Transform& transform = animatedEntity.GetComponent<Transform>();

        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetScale(Vector3(2.0f));

        SharedPtr<Model>& model = animatedEntity.AddComponent<ModelComponent>(PrimitiveType::Cube).m_handle;
        const SharedPtr<Material>& material = model->GetMeshes()[0]->GetMaterial();


        AnimationComponent& animComp = animatedEntity.AddComponent<AnimationComponent>();

      


        switch (type)
        {
        case EasingType::Linear:  transform.SetPosition(Vector3(10.0f, 0.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case EasingType::SineEaseIn:  transform.SetPosition(Vector3(10.0f, 10.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
            break;
        case EasingType::SineEaseOut:  transform.SetPosition(Vector3(10.0f, 20.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
            break;
        case EasingType::SineEaseInOut:  transform.SetPosition(Vector3(10.0f, -10.0f, 0.0f));
            material->albedoColour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            break;
        default:
            break;
        }


        animComp.AddScaleKey(Vector3(2.0f), 0.0f);
        animComp.AddScaleKey(Vector3(4.0), 3.0f, type);
        animComp.AddScaleKey(Vector3(2.0), 6.0f, type);


        animComp.loop = true;


        return animatedEntity;
    }


    Entity CreateAnimatedCubePosition(EasingType type)
    {
        Entity animatedEntity = GetCurrentScene()->CreateEntity("PositionCube");
        Transform& transform = animatedEntity.GetComponent<Transform>();

        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetScale(Vector3(2.0f));

        SharedPtr<Model>& model = animatedEntity.AddComponent<ModelComponent>(PrimitiveType::Cube).m_handle;
        const SharedPtr<Material>& material = model->GetMeshes()[0]->GetMaterial();


        AnimationComponent& animComp = animatedEntity.AddComponent<AnimationComponent>();

      


        switch (type)
        {
        case EasingType::Linear:  
            transform.SetPosition(Vector3(20.0f, 0.0f, 0.0f));

            animComp.AddPositionKey(Vector3(20.0f, 0.0f, 0.0f), 0.0f);
            animComp.AddPositionKey(Vector3(30.0f, 0.0f, 0.0f), 3.0f, type);
            animComp.AddPositionKey(Vector3(20.0f, 0.0f, 0.0f), 6.0f, type);

            material->albedoColour = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


            break;
        case EasingType::SineEaseIn:  transform.SetPosition(Vector3(20.0f, 10.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

            animComp.AddPositionKey(Vector3(20.0f, 10.0f, 0.0f), 0.0f);
            animComp.AddPositionKey(Vector3(30.0f, 10.0f, 0.0f), 3.0f, type);
            animComp.AddPositionKey(Vector3(20.0f, 10.0f, 0.0f), 6.0f, type);
            break;
        case EasingType::SineEaseOut:  transform.SetPosition(Vector3(20.0f, 20.0f, 0.0f));
            material->albedoColour = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
            animComp.AddPositionKey(Vector3(20.0f, 20.0f, 0.0f), 0.0f);
            animComp.AddPositionKey(Vector3(30.0f, 20.0f, 0.0f), 3.0f, type);
            animComp.AddPositionKey(Vector3(20.0f, 20.0f, 0.0f), 6.0f, type);


            break;
        case EasingType::SineEaseInOut:  transform.SetPosition(Vector3(20.0f, -10.0f, 0.0f));
            material->albedoColour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
            animComp.AddPositionKey(Vector3(20.0f, -10.0f, 0.0f), 0.0f);
            animComp.AddPositionKey(Vector3(30.0f, -10.0f, 0.0f), 3.0f, type);
            animComp.AddPositionKey(Vector3(20.0f, -10.0f, 0.0f), 6.0f, type);
            break;
        default:
            break;
        }


       


        animComp.loop = true;


        return animatedEntity;
    }


    Entity CreateAnimatedCubeEvent()
    {
        Entity animatedEntity = GetCurrentScene()->CreateEntity("EventCube");
        Transform& transform = animatedEntity.GetComponent<Transform>();

        transform.SetEularRotation(Vector3(0.0f, 0.0f, 0.0f));
        transform.SetScale(Vector3(2.0f));

        ModelComponent* modelComp = &animatedEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
        SharedPtr<Model>& model = modelComp->m_handle;

        const SharedPtr<Material>& material = model->GetMeshes()[0]->GetMaterial();


        AnimationComponent& animComp = animatedEntity.AddComponent<AnimationComponent>();

        FunctionScript& script = animatedEntity.AddComponent<FunctionScript>();
        script.modelCompRef = modelComp;

        transform.SetPosition(Vector3(0.0f, -20.0f, 0.0f));
        
            
        animComp.AddPositionKey(Vector3(0.0f, -20.0f, 0.0f),0.0f);
        animComp.AddPositionKey(Vector3(20.0f, -20.0f, 0.0f),3.0f, EasingType::SineEaseIn);
        animComp.AddPositionKey(Vector3(0.0f, -20.0f, 0.0f),6.0f, EasingType::SineEaseOut);

        animComp.AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), 0.0f);
        animComp.AddRotationKey(Vector3(0.0f, 0.0f, 180.0f), 3.0f, EasingType::SineEaseIn);
        animComp.AddRotationKey(Vector3(0.0f, 00.0f, 0.0f), 6.0f, EasingType::SineEaseOut);

      /*  script.AddSpacemanCallBack(animComp.AddAnimationEvent(0.0f));
       
        script.AddEaseInCallBack(animComp.AddAnimationEvent(0.01f));
        script.AddSkeletonCallBack(animComp.AddAnimationEvent(2.0f));
        
        script.AddEaseOutCallBack(animComp.AddAnimationEvent(3.0f));
        script.AddSpacemanCallBack(animComp.AddAnimationEvent(4.0f));*/

        animComp.AddAnimationEvent(0.0f, std::bind(&FunctionScript::ChangeModelMan, script));
        animComp.AddAnimationEvent(2.0f, std::bind(&FunctionScript::ChangeModelSkeleton, script));
        animComp.AddAnimationEvent(4.0f, std::bind(&FunctionScript::ChangeModelMan, script));


        animComp.AddAnimationEvent(0.1f, std::bind(&FunctionScript::ChangeColorRed, script));
        animComp.AddAnimationEvent(3.0f, std::bind(&FunctionScript::ChangeColorYellow, script));
        
       

      


        animComp.loop = true;


        return animatedEntity;
    }


    void ChangeColor()
    {

    }

    private:

        SharedPtr<Animator> animator;
        bool startAnimPlayback = false;
       
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