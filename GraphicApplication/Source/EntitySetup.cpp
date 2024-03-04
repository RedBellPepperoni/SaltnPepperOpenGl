#include "EntitySetup.h"

namespace SaltnPepperEngine
{
	
	void LoadAllModels()
	{
		SharedPtr<SkinnedModelLibrary>& skinnedModelLib = Application::GetCurrent().GetSkinnedModelLibrary();

		skinnedModelLib->LoadModel("Character", "Assets\\Models\\boblampclean.md5mesh");
		//skinnedModelLib->LoadModel("Character", "Assets\\Models\\Vanguard.dae");

	}

	void LoadAllTextures()
	{
		SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

		textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);
		textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
		textureLib->LoadTexture("vanguard", "Assets\\Textures\\vanguard.png", TextureFormat::RGBA);


	}

	Entity CreateMainCamera(Vector3 Position, Vector3 Rotation)
	{
		// Create a new Entity and add it to the ECS
		Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");

		// Add a Camera component to the entitty
		Camera& camera = cameraEntity.AddComponent<Camera>(16.0f / 10.0f, 0.01f, 2000.0f);

		// Cache the transform Reference
		Transform& transform = cameraEntity.GetComponent<Transform>();

		FlyCameraController& controller = cameraEntity.AddComponent<FlyCameraController>();

		controller.SetCamera(&camera);

		// Set the tranasform Values
		transform.SetPosition(Position);
		transform.SetEularRotation(Rotation);

		return cameraEntity;
	}
	Entity CreateParentedEntity(Vector3 Position, Vector3 Rotation)
	{
		Entity baseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BaseEntity");
		Hierarchy& hierarchyComp = baseEntity.AddComponent<Hierarchy>();
		Transform& transform = baseEntity.GetComponent<Transform>();

		transform.SetPosition(Vector3(0.0f, 0.0f, 0.0f));

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
		transform.SetEularRotation(Vector3(154.0f, -51.0f, -180.0f));
		Light& light = dirLightEntity.AddComponent<Light>();
		light.type = LightType::DirectionLight;
		light.intensity = 1.2f;

		return dirLightEntity;
	}

	Entity SpawnSkinnedCharacter(const Vector3& position)
	{
		Entity characterEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SkinnedCharacter");
		Transform& transform = characterEntity.GetComponent<Transform>();
		transform.SetPosition(position);

		SkinnedModelComponent& modelComp = characterEntity.AddComponent<SkinnedModelComponent>("Character");
		SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

		mat->SetAlbedoTexture("vanguard");

		return characterEntity;
	}


}