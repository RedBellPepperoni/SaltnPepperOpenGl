#include "SaltnPepperEngine.h"



class GraphicRuntime : public Application
{
	void OnInit()
	{
        SharedPtr<Mesh> mesh = GetModelLibrary()->LoadModel("Sphere", "Assets\\Models\\Sphere.fbx")->GetMeshes()[0];

        Entity entity = GetCurrentScene()->CreateEntity("Main Test Entity");
        Transform& transform = entity.AddComponent<Transform>();

        transform.SetPosition(Vector3(0.0f));
        transform.SetEularRotation(Vector3(0.0f));

        entity.AddComponent<MeshComponent>(mesh);
        entity.AddComponent<MeshRenderer>();





        Entity dirLightEntity = GetCurrentScene()->CreateEntity("Directional Light");
        transform = dirLightEntity.AddComponent<Transform>();

        transform.SetPosition(Vector3(0.0f));
        transform.SetEularRotation(Vector3(10.0f, 20.0f,0.0f));

        Light& light = dirLightEntity.AddComponent<Light>();
        light.direction = transform.GetForwardVector();

        light.intensity = 1.0f;
        light.color = Vector3(1.0f,1.0f,1.0f);
        light.type = LightType::DirectionLight;

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