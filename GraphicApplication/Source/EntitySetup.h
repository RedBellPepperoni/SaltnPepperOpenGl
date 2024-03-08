#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"

static int cameraCount = 0;


void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

	modelLib->LoadModel("Bed", "Assets\\Models\\Lake_Bed.fbx");
	modelLib->LoadModel("Bench", "Assets\\Models\\Lake_Bench.fbx");
	modelLib->LoadModel("Boat", "Assets\\Models\\Lake_Boat.fbx");
	modelLib->LoadModel("Branch", "Assets\\Models\\Lake_Branch.fbx");
	modelLib->LoadModel("Bucket", "Assets\\Models\\Lake_Bucket.fbx");
	modelLib->LoadModel("Crow", "Assets\\Models\\Lake_Crow.fbx");
	modelLib->LoadModel("Dock", "Assets\\Models\\Lake_Dock.fbx");
	modelLib->LoadModel("Ducks", "Assets\\Models\\Lake_Ducks.fbx");
	modelLib->LoadModel("Fence", "Assets\\Models\\Lake_Fence.fbx");
	modelLib->LoadModel("FishSchool", "Assets\\Models\\Lake_FishSchool.fbx");
	modelLib->LoadModel("Grass", "Assets\\Models\\Lake_Grass.fbx");
	modelLib->LoadModel("Ground", "Assets\\Models\\Lake_Ground.fbx");
	modelLib->LoadModel("Lamp", "Assets\\Models\\Lake_Lamp.fbx");
	modelLib->LoadModel("PlantPot", "Assets\\Models\\Lake_PlantPot.fbx");
	modelLib->LoadModel("Plants", "Assets\\Models\\Lake_Plants.fbx");
	modelLib->LoadModel("Rock", "Assets\\Models\\Lake_Rock.fbx");
	modelLib->LoadModel("Rocks", "Assets\\Models\\Lake_Rocks.fbx");
	modelLib->LoadModel("Shack", "Assets\\Models\\Lake_Shack.fbx");
	modelLib->LoadModel("Shrooms", "Assets\\Models\\Lake_Shrooms.fbx"); 
	modelLib->LoadModel("Tree", "Assets\\Models\\Lake_Tree.fbx");
	modelLib->LoadModel("Water", "Assets\\Models\\Lake_Water.fbx");


}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("pallet", "Assets\\Textures\\color.png", TextureFormat::RGBA);
	
	
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

Entity CreateAsteroidParent()
{
	Entity AsteroidParentEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Parent");
	Hierarchy& hierarchy = AsteroidParentEntity.AddComponent<Hierarchy>();

	return AsteroidParentEntity;
}

Entity CreatePointLight(Vector3 position = Vector3{ 0.0f }, float Radius = 5.0f, float Intensity = 1.0f, Vector3 color = Vector3(1.0f))
{
	Entity pointLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PointLight");
	Transform& transform = pointLightEntity.GetComponent<Transform>();
	transform.SetPosition(position);

	Light& light = pointLightEntity.AddComponent<Light>();
	light.type = LightType::PointLight;
	light.radius = Radius;
	light.intensity = Intensity;
	light.color = color;

	return pointLightEntity;
}

Entity CreateDirectionalLight()
{
	Entity dirLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Directional Light");
	Transform& transform = dirLightEntity.GetComponent<Transform>();
	transform.SetEularRotation(Vector3(154.0f, -51.0f, -180.0f));
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 1.2f;

	return dirLightEntity;
}

// Asteroid Stuff

Entity CreateMainAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateIndustrialAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Industrial_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid_Industrial");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("MiningRig");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

Entity CreateIndustrialMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Industrial_Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("IndustrialMiningRig");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

Entity CreateSecondaryAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Other_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid_Other");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateSecondaryMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Other_Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("MiningRig_Other");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

// Consoles Stuff

Entity CreateConsoleRoom(const Vector3& position = Vector3(0.0f))
{
	Entity consoleRoomEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("ConsoleRoom");
	Hierarchy& hierarchy = consoleRoomEntity.AddComponent<Hierarchy>();

	Transform& roomTransform = consoleRoomEntity.GetComponent<Transform>();
	roomTransform.SetPosition(position);

	return consoleRoomEntity;
}


Entity CreateWater(const Vector3 position = Vector3(0.0f))
{
	Entity waterEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Lake_Water");
	Hierarchy& hierarchyComp = waterEntity.AddComponent<Hierarchy>();
	Transform& transform = waterEntity.GetComponent<Transform>();
	transform.SetPosition(position);

	ModelComponent* modelComp = &waterEntity.AddComponent<ModelComponent>("Water");
	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
	//mat->m_type = MaterialType::Transparent;
	mat->m_type = MaterialType::Opaque;

	mat->SetAlbedoTexture("pallet");

	return waterEntity;
}


//Entity CreateCenterScreen(const Vector3& position = Vector3(0.0f), bool isRightScreen = true)
//{
//	std::string name = isRightScreen ? "CenterScreen_Right" : "CenterScreen_Left";
//	Entity screenEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
//	Hierarchy& hierarchyComp = screenEntity.AddComponent<Hierarchy>();
//	Transform& transform = screenEntity.GetComponent<Transform>();
//	transform.SetPosition(position);
//
//	float switchtime = isRightScreen ? 4.0f : 5.1f;
//
//	ScifiScreen& screen = screenEntity.AddComponent<ScifiScreen>(switchtime);
//
//	
//	std::string modelName = isRightScreen ? "CenterScreenRight" : "CenterScreenLeft";
//	ModelComponent* modelComp = &screenEntity.AddComponent<ModelComponent>(modelName);
//
//	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
//	mat->m_type = MaterialType::Custom;
//
//	
//	isRightScreen ? mat->SetAlbedoTexture("rightOne") : mat->SetAlbedoTexture("leftOne");
//	mat->SetMetallicTexture("noise");
//	mat->name = isRightScreen ? "Distort" : "Chromatic";
//
//	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();
//
//	screen.SetMaterialRef(mat);
//	
//	
//
//	return screenEntity;
//}



SharedPtr<Texture> CreateSecurityCamera(const Vector3& position = Vector3(0.0f),const Vector3& rotation = Vector3(0.0f))
{
	cameraCount++;

	std::string name = "SecurityCameraRotator_" + std::to_string(cameraCount);
	Entity rotatorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& rothierarchyComp = rotatorEntity.AddComponent<Hierarchy>();
	Transform& rottransform = rotatorEntity.GetComponent<Transform>();
	SecurityCamera& cameraSec = rotatorEntity.AddComponent<SecurityCamera>();

	rottransform.SetPosition(position);

	float randomYaw = Random32::Range.GetRandom(-15.0f, 15.0f);
	
	rottransform.SetEularRotation(Vector3(0.0f, randomYaw, 0.0f));

	name = "SecurityCameraEntity_" + std::to_string(cameraCount);
	Entity meshEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = meshEntity.AddComponent<Hierarchy>();
	Transform& transform = meshEntity.GetComponent<Transform>();

	transform.SetEularRotation(rotation);

	ModelComponent* modelComp = &meshEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);

	meshEntity.SetParent(rotatorEntity);

	name = "Camera_" + std::to_string(cameraCount);
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& camhierarchyComp = cameraEntity.AddComponent<Hierarchy>();
	Transform& camtransform = cameraEntity.GetComponent<Transform>();
	camtransform.SetPosition(Vector3(0.0f, 1.0f, -2.0f));
	Camera& camera = cameraEntity.AddComponent<Camera>(16.0f/9.0f,0.01f,1500.0f);

	cameraEntity.SetParent(meshEntity);

	return camera.GetRenderTexture();

}

#endif //  EntitySetup

