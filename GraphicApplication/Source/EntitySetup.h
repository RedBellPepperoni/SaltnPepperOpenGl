#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "PlayerCharacter.h"
#include "ThirdPersonCameraController.h"
#include "AIStuff/AIBaseAgent.h"
#include "AIStuff/AIManager.h"

struct PlayerLook
{
	std::string name = "Hello";
};

struct WanderPArams
{
	float primaryRadius;
	float secondaryRadius;
	float decision;
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

	modelLib->LoadModel("PlayerKnight","Assets\\Models\\PlayerKnight.fbx");
	modelLib->LoadModel("Goblin","Assets\\Models\\Goblin.fbx");
	modelLib->LoadModel("Cat","Assets\\Models\\Cat.fbx");
	modelLib->LoadModel("Spider","Assets\\Models\\Spider.fbx");
	modelLib->LoadModel("Deer","Assets\\Models\\Deer.fbx");
	modelLib->LoadModel("Sheep","Assets\\Models\\Sheep.fbx");
	modelLib->LoadModel("Rock","Assets\\Models\\Rock.fbx");
	modelLib->LoadModel("Dog","Assets\\Models\\Dog.fbx");
	modelLib->LoadModel("Rooster","Assets\\Models\\Rooster.fbx");
	modelLib->LoadModel("Bread","Assets\\Models\\Bread.fbx");

}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("player", "Assets\\Textures\\player.png", TextureFormat::RGBA);
	textureLib->LoadTexture("goblin", "Assets\\Textures\\goblin.png", TextureFormat::RGBA);
	textureLib->LoadTexture("cat", "Assets\\Textures\\cat.png", TextureFormat::RGBA);
	textureLib->LoadTexture("grass", "Assets\\Textures\\grass.png", TextureFormat::RGBA);
	textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
	textureLib->LoadTexture("spider", "Assets\\Textures\\spider.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("deer", "Assets\\Textures\\deer.jpeg", TextureFormat::RGBA);
	textureLib->LoadTexture("sheep", "Assets\\Textures\\sheep.png", TextureFormat::RGBA);
	textureLib->LoadTexture("dog", "Assets\\Textures\\dog.png", TextureFormat::RGBA);
	textureLib->LoadTexture("rock", "Assets\\Textures\\rock.png", TextureFormat::RGBA);
	textureLib->LoadTexture("rooster", "Assets\\Textures\\rooster.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("bread", "Assets\\Textures\\bread.jpg", TextureFormat::RGBA);
}

Entity CreateMainCamera(Vector3 Position = Vector3{0.0f}, Vector3 Rotation = Vector3{0.0f})
{	
	// Create a new Entity and add it to the ECS
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CameraEntity");
		
		// Add a Camera component to the entitty
	Camera& camera = cameraEntity.AddComponent<Camera>();

	// Cache the transform Reference
	Transform& transform = cameraEntity.GetComponent<Transform>();

	ThirdPersonCameraController& controller = cameraEntity.AddComponent<ThirdPersonCameraController>();

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


Entity CreatePlayerCharacter(Entity mainCamera)
{
	Entity playerbaseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerBase");
	Hierarchy& hierarchyComp = playerbaseEntity.AddComponent<Hierarchy>();
	Transform* transform = &playerbaseEntity.GetComponent<Transform>();
	PlayerCharacter& player = playerbaseEntity.AddComponent<PlayerCharacter>();
	transform->SetPosition(Vector3(-42.0f, 0.0f, 40.0f));


	PhysicsProperties properties;
	properties.collider = MakeShared<SphereCollider>(1.0f);
	properties.mass = 20.0f;
	properties.friction = 0.8f;
	properties.position = Vector3(-42.0f, 0.0f, 40.0f);
	RigidBody3D* bodyRef = playerbaseEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
	bodyRef->SetVelocity(Vector3(5.0f, 0.0f, 0.0f));

	Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerModel");
	childEntity.AddComponent<PlayerLook>();
	Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();
	Transform* childTransform = &childEntity.GetComponent<Transform>();
	childTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

	ModelComponent& modelComp = childEntity.AddComponent<ModelComponent>("PlayerKnight");
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

	mat->SetAlbedoTexture("player.png");

	mainCamera.AddComponent<Hierarchy>();
	childEntity.SetParent(playerbaseEntity);
	mainCamera.SetParent(playerbaseEntity);

	Transform* cameraTransform = &mainCamera.GetComponent<Transform>();
	cameraTransform->SetPosition(Vector3(0.0f, 4.0f, 6.0f));
	player.Init(bodyRef, childTransform, cameraTransform);

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

	transform.SetScale(Vector3(160.0f, 1.0f, 90.0f));
	transform.SetPosition(Vector3(0.0f, -0.5f, 0.0f));

	ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	
	//mat->albedoColour = Vector4(0.0f, 1.0f, 0.2f, 1.0f);
	mat->SetAlbedoTexture("snow");
	return floorEntity;
}


Entity CreateEnemyAI(AI::BehaviorState state, EnemyModel model,Vector3 position = Vector3(10.0f, 0.0f, 10.0f))
{
	Entity enemyEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Seek Enemy");
	Hierarchy& hierarchyComp = enemyEntity.AddComponent<Hierarchy>();
	Transform& transform = enemyEntity.GetComponent<Transform>();
	AI::AIBaseAgent& agent = enemyEntity.AddComponent<AI::AIBaseAgent>();

	
	transform.SetPosition(position);


	PhysicsProperties properties;
	properties.collider = MakeShared<SphereCollider>(1.0f);
	properties.mass = 20.0f;
	properties.friction = 0.8f;
	properties.position = position;
	RigidBody3D* bodyRef = enemyEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();


	

	Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Enemy Model");
	Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();
	Transform* childtransform = &childEntity.GetComponent<Transform>();

	agent.Init(bodyRef, childtransform);
	agent.m_lookTransform = &childEntity.GetComponent<Transform>();
	agent.SetBehaviour(state);

	ModelComponent* modelComp = nullptr;
	SharedPtr<Material> mat = nullptr;
	switch (model)
	{
	case GOBLIN: modelComp = &childEntity.AddComponent<ModelComponent>("Goblin");
		mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("goblin");
		break;

	case SHEEP:modelComp = &childEntity.AddComponent<ModelComponent>("Sheep");
		 mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("sheep");
		break;

	case DEER:modelComp = &childEntity.AddComponent<ModelComponent>("Deer");
		 mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("deer");
		break;

	case SPIDER:modelComp = &childEntity.AddComponent<ModelComponent>("Spider");
		 mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("spider");
		break;

	case CAT:modelComp = &childEntity.AddComponent<ModelComponent>("Cat");
		 mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("cat");
		break;

	}
	
	


	childEntity.SetParent(enemyEntity);
	

	return enemyEntity;
}



Entity CreateWanderAI(WanderPArams params, EnemyModel model, Vector3 position = Vector3(10.0f, 0.0f, 10.0f))
{
	Entity enemyEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Wander Enemy");
	Hierarchy& hierarchyComp = enemyEntity.AddComponent<Hierarchy>();
	Transform& transform = enemyEntity.GetComponent<Transform>();
	AI::AIBaseAgent& agent = enemyEntity.AddComponent<AI::AIBaseAgent>();


	transform.SetPosition(position);


	PhysicsProperties properties;
	properties.collider = MakeShared<SphereCollider>(1.0f);
	properties.mass = 20.0f;
	properties.friction = 0.8f;
	properties.position = position;
	RigidBody3D* bodyRef = enemyEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();




	Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Enemy Model");
	Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();
	Transform* childtransform = &childEntity.GetComponent<Transform>();

	agent.Init(bodyRef, childtransform);
	agent.m_lookTransform = &childEntity.GetComponent<Transform>();
	agent.SetBehaviour(AI::Wander);
	agent.SetWanderParams(params.primaryRadius, params.secondaryRadius, params.decision);

	ModelComponent* modelComp = nullptr;
	SharedPtr<Material> mat = nullptr;

	switch (model)
	{
	case GOBLIN: modelComp = &childEntity.AddComponent<ModelComponent>("Goblin");
		mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("goblin");
		break;

	case SHEEP:modelComp = &childEntity.AddComponent<ModelComponent>("Sheep");
		mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("sheep");
		break;

	case CAT:modelComp = &childEntity.AddComponent<ModelComponent>("Cat");
		mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
		mat->SetAlbedoTexture("cat");
		break;

	}




	childEntity.SetParent(enemyEntity);


	return enemyEntity;
}


void CreateBreadScene()
{
	//Entity Bread
}

void CreateBread(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{

}

#endif //  EntitySetup

