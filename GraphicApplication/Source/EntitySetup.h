#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "PlayerCharacter.h"
#include "AnimationDefinitions.h"


static int platformId = 0;

struct PlayerLook
{
	std::string name = "Hello";
};

enum EnemyModel : uint8_t
{
	GOBLIN,
	SHEEP,
	DEER,
	SPIDER,
	CAT
	
};

void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

	modelLib->LoadModel("Cat","Assets\\Models\\Cat.fbx");
	modelLib->LoadModel("HandLeft","Assets\\Models\\Hand_Left.fbx");
	modelLib->LoadModel("HandRight","Assets\\Models\\Hand_Right.fbx");
	modelLib->LoadModel("Sheep","Assets\\Models\\Sheep.fbx");

}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();


	textureLib->LoadTexture("cat", "Assets\\Textures\\cat.png", TextureFormat::RGBA);
	textureLib->LoadTexture("grass", "Assets\\Textures\\grass2.png", TextureFormat::RGBA);
	textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
	textureLib->LoadTexture("sheep", "Assets\\Textures\\sheep.png", TextureFormat::RGBA);
}

Entity CreateMainCamera(Vector3 Position = Vector3{0.0f}, Vector3 Rotation = Vector3{0.0f})
{	
	// Create a new Entity and add it to the ECS
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");
		
		// Add a Camera component to the entitty
	Camera& camera = cameraEntity.AddComponent<Camera>();

	// Cache the transform Reference
	Transform& transform = cameraEntity.GetComponent<Transform>();

	// Add the Camera Controller
	FirstPersonCameraController& controller = cameraEntity.AddComponent<FirstPersonCameraController>();

	// Set the camera Reference
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


Entity CreatePlayerCharacter(Entity mainCamera, Entity leftHand, Entity rightHand, Vector3 Position = Vector3(0.0f,3.0f,0.0f))
{
	Entity playerbaseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerBase");
	Hierarchy& hierarchyComp = playerbaseEntity.AddComponent<Hierarchy>();
	Transform* transform = &playerbaseEntity.GetComponent<Transform>();
	PlayerCharacter& player = playerbaseEntity.AddComponent<PlayerCharacter>();
	transform->SetPosition(Position);


	PhysicsProperties properties;
	properties.collider = MakeShared<SphereCollider>(1.0f);
	properties.mass = 20.0f;
	properties.friction = 0.8f;
	properties.position = Position;
	properties.elasticity = 0.1f;
	RigidBody3D* bodyRef = playerbaseEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
	bodyRef->SetVelocity(Vector3(5.0f, 0.0f, 0.0f));

	Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerCamera");
	childEntity.AddComponent<PlayerLook>();
	Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();
	Transform* childTransform = &childEntity.GetComponent<Transform>();
	childTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

	//ModelComponent& modelComp = childEntity.AddComponent<ModelComponent>("PlayerKnight");
	//SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

	//mat->SetAlbedoTexture("player.png");

	mainCamera.AddComponent<Hierarchy>();
	leftHand.AddComponent<Hierarchy>();
	rightHand.AddComponent<Hierarchy>();


	childEntity.SetParent(playerbaseEntity);
	mainCamera.SetParent(playerbaseEntity);

	leftHand.SetParent(mainCamera);
	rightHand.SetParent(mainCamera);

	Transform* cameraTransform = &mainCamera.GetComponent<Transform>();
	Transform* leftHandTransform = &leftHand.GetComponent<Transform>();
	cameraTransform->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
	player.Init(bodyRef, cameraTransform);



	return playerbaseEntity;
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

Entity CreateBaseFloor()
{
	Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Floor");
	Transform& transform = floorEntity.GetComponent<Transform>();

	transform.SetScale(Vector3(100.0f, 1.0f, 100.0f));
	transform.SetPosition(Vector3(0.0f, -0.5f, 0.0f));

	ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	
	PhysicsProperties properties;
	properties.collider = MakeShared<BoxCollider>(Vector3(50.0f,0.5f,50.0f));
	properties.mass = 2000.0f;
	properties.friction = 0.8f;
	properties.stationary = true;
	properties.isStatic = true;
	properties.position = Vector3(0.0f);
	RigidBody3D* bodyRef = floorEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
	bodyRef->SetVelocity(Vector3(5.0f, 0.0f, 0.0f));

	//mat->albedoColour = Vector4(0.0f, 1.0f, 0.2f, 1.0f);
	mat->SetAlbedoTexture("snow");
	return floorEntity;
}


Entity CreateHand(bool Left)
{
	std::string name = Left ? "Left" : "Right";

	Entity handEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Hand_" + name );
	Transform& transform = handEntity.GetComponent<Transform>();
	
	Vector3 position = Left ? Vector3(-0.7f, -0.5f, 0.0f) : Vector3(0.7f, -0.5f, 0.0f);

	transform.SetPosition(position);

	ModelComponent& modelComp = handEntity.AddComponent<ModelComponent>(Left? "HandLeft" : "HandRight");
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->albedoColour = Vector4(0.59f, 0.44f, 0.33f, 1.0f);


	


	return handEntity;
}

void CreatePlatform(Vector3 position, Vector3 size)
{
	Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Platform_" + std::to_string(platformId));
	Transform& transform = floorEntity.GetComponent<Transform>();

	Vector3 halfSize = Vector3(size.x / 2, size.y / 2, size.z / 2);

	transform.SetScale(size);
	transform.SetPosition(position);
	//transform.SetRotation(Quaternion(Vector3(0.0f)));

	ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

	PhysicsProperties properties;
	properties.collider = MakeShared<BoxCollider>(halfSize);
	properties.mass = 2000.0f;
	properties.friction = 0.8f;
	properties.stationary = true;
	properties.isStatic = true;
	properties.position = position;

	//RigidBody3D* bodyRef = floorEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
	floorEntity.AddComponent<RigidBodyComponent>(properties);
	/*bodyRef->SetPosition(position);
	bodyRef->SetRotation(Quaternion(Vector3(0.0f)));*/

	//mat->albedoColour = Vector4(0.0f, 1.0f, 0.2f, 1.0f);
	mat->SetAlbedoTexture("grass");

	platformId++;
	//return floorEntity;
}


#endif //  EntitySetup

