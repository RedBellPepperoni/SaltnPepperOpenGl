#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"

static int cameraCount = 0;
static int tvCount = 0;
static int boxCount = 0;

struct FishSchool
{
	bool isMoving = true;;
};

enum class DistortType : uint8_t
{
	CHROMATIC,
	DISTORT
};

enum class LakeModel : uint8_t
{
	BED,
	BENCH,
	BOAT,
	BRANCH,
	BUCKET,
	CROW,
	DOCK,
	DUCKS,
	FENCE,
	FISHSCHOOL,
	GRASS,
	GROUND,
	LAMP,
	PLANTPOT,
	PLANTS,
	ROCK,
	ROCKS,
	SHACK,
	SHROOMS,
	TREE,
	WATER,
	TABLE,
	STOOL,
	POLE
};


static std::array<std::string, 24> LakeModelString =

{
	"Bed",
	"Bench",
	"Boat",
	"Branch",
	"Bucket",
	"Crow",
	"Dock",
	"Ducks",
	"Fence",
	"FishSchool",
	"Grass",
	"Ground",
	"Lamp",
	"PlantPot",
	"Plants",
	"Rock",
	"Rocks",
	"Shack",
	"Shrooms",
	"Tree",
	"Water",
	"Table",
	"Stool",
	"Pole"
};




void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();
	SharedPtr<SkinnedModelLibrary>& skinnedmodelLib = Application::GetCurrent().GetSkinnedModelLibrary();

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
	modelLib->LoadModel("Pole", "Assets\\Models\\Lake_Pole.fbx");

	modelLib->LoadModel("Table", "Assets\\Models\\Lake_Table.fbx");
	modelLib->LoadModel("Stool", "Assets\\Models\\Lake_Stool.fbx");
	modelLib->LoadModel("TV", "Assets\\Models\\TV.fbx");
	modelLib->LoadModel("TVScreen", "Assets\\Models\\TVScreen.fbx");

	modelLib->LoadModel("Cam", "Assets\\Models\\Cam.fbx");
	modelLib->LoadModel("CamBase", "Assets\\Models\\CamBase.fbx");


	skinnedmodelLib->LoadModel("SCharacter", "Assets\\Models\\aj.dae");
	skinnedmodelLib->LoadModel("RacerCharacter", "Assets\\Models\\racer.dae");



}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("pallet", "Assets\\Textures\\color.png", TextureFormat::RGBA);
	textureLib->LoadTexture("water", "Assets\\Textures\\water.png", TextureFormat::RGBA);
	textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("skinned", "Assets\\Textures\\aj.png", TextureFormat::RGBA);
	textureLib->LoadTexture("racer", "Assets\\Textures\\racer.png", TextureFormat::RGBA);
	textureLib->LoadTexture("racerNormal", "Assets\\Textures\\racerNormal.png", TextureFormat::RGBA);
	
	
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
	transform.SetEularRotation(Vector3(-11.4192f, 51.3504f, -7.0023f));
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 1.7f;

	return dirLightEntity;
}



Entity CreateEntity(const LakeModel model, const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const Vector3& scale = Vector3(1.0f))
{
	 
	const std::string Modelname = LakeModelString[(uint8_t)model];
	std::string name = "Lake_" + Modelname;


	Entity entity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name); 
	Hierarchy& hierarchyComp = entity.AddComponent<Hierarchy>();
	Transform& transform = entity.GetComponent<Transform>();
	transform.SetPosition(position);
	transform.SetEularRotation(rotation);
	transform.SetScale(scale);

	ModelComponent* modelComp = &entity.AddComponent<ModelComponent>(Modelname);


	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();

	



	if (model == LakeModel::WATER)
	{
		mat->m_type = MaterialType::Transparent;
		mat->metallic = 0.9f;
		mat->roughness = 0.6f;
		mat->albedoColour = Vector4(0.0f, 0.0f, 0.0f, 0.75f);

		//mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("water");
	}

	else
	{
		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("pallet");
		mat->albedoMapFactor = 1.0f;
	}


	if (model == LakeModel::FISHSCHOOL)
	{
		Entity parententity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name + "_Parent");
		Hierarchy& hierarchy = parententity.AddComponent<Hierarchy>();
		Transform& parenttransform = parententity.GetComponent<Transform>();
		parententity.AddComponent<FishSchool>();


		parenttransform.SetPosition(position);
		parenttransform.SetEularRotation(rotation);
		parenttransform.SetScale(scale);

		transform.SetPosition(Vector3(-0.7956f, 0.0f, 2.9702f));
		transform.SetEularRotation(Vector3(0.0f, -14.9953f,0.0f));
		transform.SetScale(Vector3(1.0f));



		entity.SetParent(parententity);

		return parententity;

	}


	return entity;
}



void CreateTV(SharedPtr<Texture>& camtex,const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f),const DistortType type = DistortType::DISTORT)
{
	tvCount++;
	Entity parententity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
	Hierarchy& parenthierarchy = parententity.AddComponent<Hierarchy>();
	Transform& parenttransform = parententity.GetComponent<Transform>();
	parenttransform.SetPosition(position);
	parenttransform.SetEularRotation(rotation);
	parenttransform.SetScale(Vector3(0.5f,0.4f,0.4f));
	ModelComponent& parentModel = parententity.AddComponent<ModelComponent>("TV");


	SharedPtr<Material> mat = parentModel.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");
	mat->m_type = MaterialType::Opaque;


	Entity screenentity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
	Hierarchy& hierarchy = screenentity.AddComponent<Hierarchy>();
	Transform& screentransform = screenentity.GetComponent<Transform>();
	screentransform.SetPosition(Vector3(0.0f,0.0f, -0.1705f));
	screentransform.SetEularRotation(Vector3(179.1376f, -1.6693f, -179.9750f));
	screentransform.SetScale(Vector3(0.7010f, 0.4835f, 0.9859f));

	ModelComponent& screenModel = screenentity.AddComponent<ModelComponent>(PrimitiveType::Quad);

	SharedPtr<Material> tvMat = screenModel.m_handle->GetMeshes()[0]->GetMaterial();
	
	tvMat->textureMaps.albedoMap = camtex;
	tvMat->m_type = MaterialType::Custom;
	tvMat->SetMetallicTexture("noise");
	tvMat->name = type == DistortType::DISTORT ? "Distort" : "Chromatic";

	

	screenentity.SetParent(parententity);



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



SharedPtr<Texture>& CreateSecurityCamera(const Vector3& position = Vector3(0.0f),const Vector3& rotation = Vector3(0.0f), const Vector3 camRotation = Vector3(0.0f),bool Moving = false)
{
	cameraCount++;
	std::string name = "Cam_Base_" + std::to_string(cameraCount);

	Entity baseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& basehierarchyComp = baseEntity.AddComponent<Hierarchy>();
	Transform& basetransform = baseEntity.GetComponent<Transform>();
	basetransform.SetPosition(position);
	basetransform.SetEularRotation(rotation);
	ModelComponent& baseModel = baseEntity.AddComponent<ModelComponent>("CamBase");
	SharedPtr<Material> basemat = baseModel.m_handle->GetMeshes()[0]->GetMaterial();
	basemat->SetAlbedoTexture("metal");



	name = "Cam_Rotator_" + std::to_string(cameraCount);
	Entity rotatorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& rothierarchyComp = rotatorEntity.AddComponent<Hierarchy>();
	Transform& rottransform = rotatorEntity.GetComponent<Transform>();
	rottransform.SetPosition(Vector3(0.0f, 0.0994f, -0.3175f));
	rottransform.SetEularRotation(camRotation);

	if (Moving)
	{
		rotatorEntity.AddComponent<SecurityCamera>();
	}

	rotatorEntity.SetParent(baseEntity);





	name = "Cam_Model_" + std::to_string(cameraCount);
	Entity meshEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = meshEntity.AddComponent<Hierarchy>();
	Transform& transform = meshEntity.GetComponent<Transform>();
	transform.SetEularRotation(Vector3(-16.02f, 0.0f, 0.0f));
	ModelComponent& modelComp = meshEntity.AddComponent<ModelComponent>("Cam");

	meshEntity.SetParent(rotatorEntity);

	name = "Camera_" + std::to_string(cameraCount);
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& camhierarchyComp = cameraEntity.AddComponent<Hierarchy>();
	Transform& camtransform = cameraEntity.GetComponent<Transform>();
	camtransform.SetPosition(Vector3(0.0f, 0.0f, -0.5f));
	Camera* camera = &cameraEntity.AddComponent<Camera>(16.0f/9.0f,0.01f,500.0f);

	cameraEntity.SetParent(meshEntity);

	camera->m_name = "Camera_" + std::to_string(cameraCount);


	//LOG_WARN("Name: {0}", std::to_string((uint64_t)texture.get()));

	return camera->GetRenderTexture();

}


Entity CreateSkinnedCharatcer(const Vector3& position = Vector3{ 0.0f }, const Vector3& scale = Vector3{ 1.0f })
{
	Entity skinnedEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SkinnedMesh");
	Transform& transform = skinnedEntity.GetComponent<Transform>();

	transform.SetPosition(position);
	transform.SetScale(scale);

	SkinnedModelComponent& modelComp = skinnedEntity.AddComponent<SkinnedModelComponent>("RacerCharacter");

	SharedPtr<SkinnedAnimation> idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Idle","Assets\\Models\\raceridle.dae",modelComp.m_handle);
	SharedPtr<SkinnedAnimation> walkanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Walk","Assets\\Models\\racerwalk.dae",modelComp.m_handle);
	SharedPtr<SkinnedAnimation> rightanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_RightStrafe","Assets\\Models\\racerrightwalk.dae",modelComp.m_handle);
	SharedPtr<SkinnedAnimation> leftanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe","Assets\\Models\\racerleftwalk.dae",modelComp.m_handle);
	SharedPtr<SkinnedAnimation> danceanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe","Assets\\Models\\racerdance.dae",modelComp.m_handle);

	AnimatorComponent& animComp = skinnedEntity.AddComponent<AnimatorComponent>();
	animComp.GetAnimator()->AddAnimation("Idle", idleanim);
	animComp.GetAnimator()->AddAnimation("Walk", walkanim);
	animComp.GetAnimator()->AddAnimation("StrafeRight", rightanim);
	animComp.GetAnimator()->AddAnimation("StrafeLeft", leftanim);
	animComp.GetAnimator()->AddAnimation("Dance", danceanim);




	for (SharedPtr<Mesh>& mesh : modelComp.m_handle->GetMeshes())
	{
		mesh->GetMaterial()->SetAlbedoTexture("racer");
		mesh->GetMaterial()->SetNormalTexture("racerNormal");
		mesh->GetMaterial()->normalMapFactor = 1.0f;
	}

	animComp.GetAnimator()->PlayAnimation(idleanim);

	return skinnedEntity;

}

Entity CreatePhysicsBox(const Vector3& position)
{
	std::string name = "Box_" + std::to_string(boxCount);
	Entity boxEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	/*Transform& boxTransform = boxEntity.GetComponent<Transform>();

	boxTransform.SetPosition(position);

	BoundingBox bounds{ Vector3(-0.5f),Vector3(0.5f)};

	BoxCollider* collider = boxEntity.AddComponent<BoxColliderComponent>().GetCollider();

	collider->Init(bounds);

	RigidBody* body = boxEntity.AddComponent<RigidBodyComponent>().GetRigidBody().get();
	body->Init(boxTransform, collider);
	
	body->MakeDynamic();
	body->SetMass(100.0f);
	body->SetFriction(1.0f);
	body->SetRollingFriction(0.01f);
	body->SetAngularForceFactor(Vector3(0.1f));
	body->UpdateCollider(collider);
	body->SetActivationState(ActivationState::ACTIVETAG);

	

	ModelComponent& modelComp = boxEntity.AddComponent<ModelComponent>("Crow");*/

	return boxEntity;

}


Entity CreatePhysicsTest(const Vector3& position)
{
	std::string name = "Test_" + std::to_string(boxCount);
	Entity boxEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Transform& boxTransform = boxEntity.GetComponent<Transform>();

	

	boxTransform.SetPosition(position);

	/*BoundingBox bounds{ Vector3(-0.5f),Vector3(0.5f) };

	BoxCollider* collider = boxEntity.AddComponent<BoxColliderComponent>().GetCollider();

	collider->Init(bounds);*/

	RShape* shape = &boxEntity.AddComponent<RShape>();
	RigidBody* body = &boxEntity.AddComponent<RigidBody>(position, shape->GetShape());
	//body->GetBody()->setGravity(btVector3(0.0f,-9.81f,0.0f));
	body->GetBody()->activate(true);
	bool kin = body->GetBody()->isKinematicObject();
	/*RigidBody* body = boxEntity.AddComponent<RigidBodyComponent>().GetRigidBody().get();
	body->Init(boxTransform, collider);

	body->MakeDynamic();
	body->SetMass(100.0f);
	body->SetFriction(1.0f);
	body->SetRollingFriction(0.01f);
	body->SetAngularForceFactor(Vector3(0.1f));
	body->UpdateCollider(collider);
	body->SetActivationState(ActivationState::ACTIVETAG);*/



	ModelComponent& modelComp = boxEntity.AddComponent<ModelComponent>("Crow");

	return boxEntity;

}

#endif //  EntitySetup

