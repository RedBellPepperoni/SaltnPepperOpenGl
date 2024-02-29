#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "Engine/Core/Physics/SoftBody/VerletCloth.h"
#include "Simulation.h"
static int cameraCount = 0;
using namespace Physics;

void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

	/*modelLib->LoadModel("Floor", "Assets\\Models\\Floor.fbx");
	modelLib->LoadModel("Ceiling", "Assets\\Models\\Ceiling.fbx");
	modelLib->LoadModel("Seat", "Assets\\Models\\Prop_Seat.fbx");*/


}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();


	SharedPtr<Texture> noise = textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	noise->SetWarping(TextureWraping::REPEAT);


	textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);

	
	
}

Entity CreateMainCamera(Vector3 Position = Vector3{0.0f}, Vector3 Rotation = Vector3{0.0f})
{	
	// Create a new Entity and add it to the ECS
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");
		
		// Add a Camera component to the entitty
	Camera& camera = cameraEntity.AddComponent<Camera>(16.0f/10.0f,0.01f,2000.0f);

	// Cache the transform Reference
	Transform& transform = cameraEntity.GetComponent<Transform>();

	FlyCameraController& controller = cameraEntity.AddComponent<FlyCameraController>();

	controller.SetCamera(&camera);

	// Set the tranasform Values
	transform.SetPosition(Position);
	transform.SetEularRotation(Rotation);

	return cameraEntity;
}

Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f })
{
	Entity baseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BaseEntity");
	Hierarchy& hierarchyComp = baseEntity.AddComponent<Hierarchy>();
	Transform& transform = baseEntity.GetComponent<Transform>();

	transform.SetPosition(Vector3(0.0f,0.0f,0.0f));

	Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("ChildEntity");
	Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();

	Transform& childtransform = childEntity.GetComponent<Transform>();

	childtransform.SetPosition(Vector3(0.0f, 5.0f, 0.0f));


	childEntity.SetParent(baseEntity);

	return baseEntity;
}

Entity CreateDirectionalLight()
{
	Entity dirLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Directional Light");
	Transform& transform = dirLightEntity.GetComponent<Transform>();
	transform.SetEularRotation(Vector3(20.0f,60.0f,0.0f));
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 1.2f;

	return dirLightEntity;
}

using namespace Verlet;

Entity CreateCloth(const Vector3& position)
{
	/*Entity clothEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Cloth");
	Transform& clothTransform = clothEntity.GetComponent<Transform>();

	ClothComponent& clothComp = clothEntity.AddComponent<ClothComponent>(32);

	return clothEntity;*/

	Entity clothEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Cloth");
	Transform& clothTransform = clothEntity.GetComponent<Transform>();

	VerletClothComponent& clothComp = clothEntity.AddComponent<VerletClothComponent>(32,Vector2(5.0f,3.0f));

	clothComp.clothsim->OnInit(32, Vector2(10.0f, 6.0f));

	return clothEntity;
}

// Asteroid Stuff

#endif //  EntitySetup

