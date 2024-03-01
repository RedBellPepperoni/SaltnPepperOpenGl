#include "EntitySetup.h"

namespace SaltnPepperEngine
{
	void LoadAllModels()
	{
		SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

		modelLib->LoadModel("Floor", "Assets\\Models\\DungeonFloor.fbx");
		modelLib->LoadModel("Wall", "Assets\\Models\\DungeonWall.fbx");
		//modelLib->LoadModel("Seat", "Assets\\Models\\Prop_Seat.fbx");
	}

	void LoadAllTextures()
	{
		SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();



		textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
		textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);


	}
	Entity CreateMainCamera(Vector3 Position, Vector3 Rotation)
	{
		// Create a new Entity and add it to the ECS
		Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");

		// Add a Camera component to the entitty
		Camera& camera = cameraEntity.AddComponent<Camera>(16.0f / 10.0f, 0.1f, 500.0f);

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
	Entity CreateWall(const Vector3& position)
	{
		Entity wallEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Wall_");
		Transform& transform = wallEntity.GetComponent<Transform>();
		transform.SetPosition(position);

		ModelComponent& comp = wallEntity.AddComponent<ModelComponent>("Wall");
		SharedPtr<Material>& mat = comp.m_handle->GetMeshes()[0]->GetMaterial();

		mat->SetAlbedoTexture("metal");

		return wallEntity;
	}
	Entity CreateFloor(const Vector3& position)
	{
		Entity wallEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Floor_");
		Transform& transform = wallEntity.GetComponent<Transform>();
		transform.SetPosition(position);

		ModelComponent& comp = wallEntity.AddComponent<ModelComponent>("Floor");
		SharedPtr<Material>& mat = comp.m_handle->GetMeshes()[0]->GetMaterial();

		mat->SetAlbedoTexture("snow");

		return wallEntity;
		
		
	}
}