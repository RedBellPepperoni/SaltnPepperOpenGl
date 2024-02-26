#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"




void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

	modelLib->LoadModel("Asteroid","Assets\\Models\\Asteroid.fbx");
	modelLib->LoadModel("Asteroid_Industrial","Assets\\Models\\Asteroid_Industrial.fbx");
	modelLib->LoadModel("MiningRig","Assets\\Models\\Asteroid_MiningRig.fbx");
	modelLib->LoadModel("IndustrialMiningRig","Assets\\Models\\Asteroid_Industrial_Mine.fbx");


}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("asteroid", "Assets\\Textures\\asteroid.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("spaceship", "Assets\\Textures\\spaceship.png", TextureFormat::RGBA);
	textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
	
}

Entity CreateMainCamera(Vector3 Position = Vector3{0.0f}, Vector3 Rotation = Vector3{0.0f})
{	
	// Create a new Entity and add it to the ECS
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");
		
		// Add a Camera component to the entitty
	Camera& camera = cameraEntity.AddComponent<Camera>();

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


Entity CreateDirectionalLight()
{
	Entity dirLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Directional Light");
	Transform& transform = dirLightEntity.GetComponent<Transform>();
	transform.SetEularRotation(Vector3(-36.0f, 0.0f, 0.0f));
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 1.2f;

	return dirLightEntity;
}


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
	mat->SetAlbedoTexture("snow");

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
	mat->SetAlbedoTexture("snow");

	return miningEntity;
}



SharedPtr<Material> CreateMonitor(const Vector3& position = Vector3(0.0f))
{
	Entity tvEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
	Hierarchy& hierarchyComp = tvEntity.AddComponent<Hierarchy>();
	Transform& transform = tvEntity.GetComponent<Transform>();

	transform.SetPosition(position);

	ModelComponent* modelComp = &tvEntity.AddComponent<ModelComponent>(PrimitiveType::Quad);
	
	SharedPtr<Material> mat  = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
	
	return mat;
}


Camera* CreateSecurityCamera(const Vector3& position)
{
	Entity meshEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SecuritytCamera");
	Hierarchy& hierarchyComp = meshEntity.AddComponent<Hierarchy>();
	Transform& transform = meshEntity.GetComponent<Transform>();

	transform.SetPosition(position);

	ModelComponent* modelComp = &meshEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);


	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SecondaryCamera");
	Hierarchy& camhierarchyComp = cameraEntity.AddComponent<Hierarchy>();
    Transform& camtransform = cameraEntity.GetComponent<Transform>();
	camtransform.SetPosition(Vector3(0.0f,1.0f,-2.0f));
	Camera* camera = &cameraEntity.AddComponent<Camera>();

	cameraEntity.SetParent(meshEntity);

	return camera;

}

#endif //  EntitySetup

