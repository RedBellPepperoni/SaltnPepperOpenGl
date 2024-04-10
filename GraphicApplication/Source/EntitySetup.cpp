#include "EntitySetup.h"


std::vector<std::string> EntitySetup::SubwayModelString = {
	
	"SUB_TEST",
	"SUB_PLATFORM",
	"SUB_PLAT_BACKWALL",
	"SUB_PLAT_RIGHTWALL",
	"SUB_PLAT_LEFTWALL",
	"SUB_PLAT_PILLAR",
	"SUB_TUNNEL_WALL",
	"SUB_TUNNEL_PILLAR"
	
};


void EntitySetup::LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();
	SharedPtr<SkinnedModelLibrary>& skinnedmodelLib = Application::GetCurrent().GetSkinnedModelLibrary();

	// Misc
	modelLib->LoadModel("Crow", "Assets\\Models\\Lake_Crow.fbx");
	

	// Skinned Stuff
	skinnedmodelLib->LoadModel("Gun_Pistol", "Assets\\Models\\GUN_PISTOL.fbx");
	skinnedmodelLib->LoadModel("WarZombie", "Assets\\Models\\WarZombie.dae");


	// Static Meshes


	modelLib->LoadModel("SUB_TEST", "Assets\\Models\\Subway_Test.fbx");
	modelLib->LoadModel("SUB_PLATFORM", "Assets\\Models\\Subway_Platform.fbx");
	modelLib->LoadModel("SUB_PLAT_BACKWALL", "Assets\\Models\\Subway_Platform_BackWall.fbx");
	modelLib->LoadModel("SUB_PLAT_RIGHTWALL", "Assets\\Models\\Subway_Platform_WallRight.fbx");
	modelLib->LoadModel("SUB_PLAT_LEFTWALL", "Assets\\Models\\Subway_Platform_WallLeft.fbx");
	modelLib->LoadModel("SUB_PLAT_PILLAR", "Assets\\Models\\Subway_Pillar.fbx");

	modelLib->LoadModel("SUB_TUNNEL_WALL", "Assets\\Models\\Subway_Tunnel_Wall.fbx");
	modelLib->LoadModel("SUB_TUNNEL_PILLAR", "Assets\\Models\\Subway_Tunnel_Pillar.fbx");


}

void EntitySetup::LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	
	textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);

	textureLib->LoadTexture("fphand", "Assets\\Textures\\hand.png", TextureFormat::RGBA);
	textureLib->LoadTexture("fppistol", "Assets\\Textures\\pistol.png", TextureFormat::RGBA);

	textureLib->LoadTexture("zombieOneDiffuse", "Assets\\Textures\\zombie_D.png", TextureFormat::RGBA);
	textureLib->LoadTexture("zombieOneNormal", "Assets\\Textures\\zombie_N.png", TextureFormat::RGBA);


	// ============================= STRUCTURE TEXTURES ======================
	textureLib->LoadTexture("Struct_Diffuse", "Assets\\Textures\\M_Structure_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Struct_Normal", "Assets\\Textures\\M_Structure_normal.png", TextureFormat::RGBA);

	// ============================= STAIRS TEXTURES ======================
	textureLib->LoadTexture("Stairs_Diffuse", "Assets\\Textures\\M_Stairs_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Stairs_Normal", "Assets\\Textures\\M_Stairs_normal.png", TextureFormat::RGBA);


	// ============================= ADDITION TEXTURES ======================
	textureLib->LoadTexture("Addition_Diffuse", "Assets\\Textures\\M_Addition_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Addition_Normal", "Assets\\Textures\\M_Addition_normal.png", TextureFormat::RGBA);


	// ============================= POSTERS TEXTURES ======================
	textureLib->LoadTexture("Poster_Diffuse", "Assets\\Textures\\M_Posters_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Poster_Normal", "Assets\\Textures\\M_Posters_normal.png", TextureFormat::RGBA);

	// ============================= SUBWAY ASSETS TEXTURES ======================
	textureLib->LoadTexture("SubwayAssets_Diffuse", "Assets\\Textures\\M_SubwayAssets_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("SubwayAssets_Normal", "Assets\\Textures\\M_SubwayAssets_normal.png", TextureFormat::RGBA);

	// ============================= TICKET MACHINE TEXTURES ======================
	textureLib->LoadTexture("Ticketmachine_Diffuse", "Assets\\Textures\\M_TicketMachine_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Ticketmachine_Normal", "Assets\\Textures\\M_TicketMachine_normal.png", TextureFormat::RGBA);


	// ============================= TUNNEL TEXTURES ======================
	textureLib->LoadTexture("Tunnel_Diffuse", "Assets\\Textures\\M_Tunnel_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Tunnel_Normal", "Assets\\Textures\\M_Tunnel_normal.png", TextureFormat::RGBA);


	// ============================= TURNSTILE TEXTURES ======================
	textureLib->LoadTexture("Turnstile_Diffuse", "Assets\\Textures\\M_Turnstile_baseColor.png", TextureFormat::RGBA);
	textureLib->LoadTexture("Turnstile_Normal", "Assets\\Textures\\M_Turnstile_normal.png", TextureFormat::RGBA);

}

void EntitySetup::LoadAllAudio()
{
	SharedPtr<AudioLibrary>& audioLib = Application::GetCurrent().GetAudioLibrary();

	audioLib->LoadAudio("zombieidle1","Assets\\Audio\\zombie_idle_1.mp3");
	audioLib->LoadAudio("zombieattack1","Assets\\Audio\\zombie_attack_1.mp3");
	audioLib->LoadAudio("zombiedeath1","Assets\\Audio\\zombie_dying_1.mp3");
	audioLib->LoadAudio("zombiedeath2","Assets\\Audio\\zombie_dying_2.mp3");
	audioLib->LoadAudio("zombiealert1","Assets\\Audio\\zombie_alert_1.mp3");

}

Entity EntitySetup::CreateMainCamera(Vector3 Position, Vector3 Rotation)
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

Entity EntitySetup::CreateDirectionalLight(const Vector3& rotation)
{
	Entity dirLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Directional Light");
	Transform& transform = dirLightEntity.GetComponent<Transform>();
	transform.SetEularRotation(rotation);
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 2.7f;

	return dirLightEntity;
}

void EntitySetup::CreateTV(SharedPtr<Texture>& camtex, const Vector3& position, const Vector3& rotation, const DistortType type)
{
	tvCount++;
	Entity parententity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
	Hierarchy& parenthierarchy = parententity.AddComponent<Hierarchy>();
	Transform& parenttransform = parententity.GetComponent<Transform>();
	parenttransform.SetPosition(position);
	parenttransform.SetEularRotation(rotation);
	parenttransform.SetScale(Vector3(0.5f, 0.4f, 0.4f));
	ModelComponent& parentModel = parententity.AddComponent<ModelComponent>("TV");


	SharedPtr<Material> mat = parentModel.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");
	mat->m_type = MaterialType::Opaque;


	Entity screenentity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
	Hierarchy& hierarchy = screenentity.AddComponent<Hierarchy>();
	Transform& screentransform = screenentity.GetComponent<Transform>();
	screentransform.SetPosition(Vector3(0.0f, 0.0f, -0.1705f));
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

Entity EntitySetup::CreateStaticEntity(const SubwayModel model, const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	const std::string Modelname = SubwayModelString[(uint8_t)model];
	std::string name = "Subway_" + Modelname;


	Entity entity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = entity.AddComponent<Hierarchy>();
	Transform& transform = entity.GetComponent<Transform>();
	transform.SetPosition(position);
	transform.SetEularRotation(rotation);
	transform.SetScale(scale);

	ModelComponent* modelComp = &entity.AddComponent<ModelComponent>(Modelname);


	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();


	switch (model)
	{
	case SubwayModel::TEST:
		break;

	case SubwayModel::PLATFROM_BACKWALL:
	case SubwayModel::PLATFROM_RIGHTWALLL:
	case SubwayModel::PLATFROM_LEFTWALLL:
	case SubwayModel::PLATFROM: 

		AssignMaterial(mat, SubwayMaterial::MAT_STRUCTURE);
		break;

	case SubwayModel::PLATFROM_PILLAR:

		AssignMaterial(mat, SubwayMaterial::MAT_STAIRS);

		break;


	case SubwayModel::TUNNEL_WALL:
	case SubwayModel::TUNNEL_PILLAR:

		AssignMaterial(mat, SubwayMaterial::MAT_TUNNEL);


	
	}
	


	


	return entity;
}

RigidBody* EntitySetup::CreatePhysicsFloor(const Vector3& position, const Vector3& rotation)
{
	Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Physics_Floor");
	Transform& floorTransform = floorEntity.GetComponent<Transform>();



	floorTransform.SetPosition(position);
	floorTransform.SetEularRotation(rotation);

	//ModelComponen& model = floorEntity.AddComponent<ModelComponent>

	BoundingBox bounds{ Vector3(-20.0f,-0.5f,-40.0f),Vector3(20.0f,0.5f,40.0f) };
	BoxCollider& shape = floorEntity.AddComponent<BoxCollider>();
	shape.Init(bounds);
	RigidBody* body = floorEntity.AddComponent<RigidBodyComponent>(floorTransform, shape.GetShape()).GetRigidBody();
	body->MakeStatic();
	body->SetEntityId(floorEntity);
	body->SetBounceFactor(0.5f);
	body->SetFriction(0.8f);

	/*Entity floorDisplay = Application::GetCurrent().GetCurrentScene()->CreateEntity("Physics_Floor_Display");
	floorDisplay.GetComponent<Transform>().SetScale(Vector3(100.0f, 1.0f, 100.0f));
	floorDisplay.GetComponent<Transform>().SetPosition(Vector3(0.0f, -0.5f, 0.0f));
	ModelComponent& model = floorDisplay.AddComponent<ModelComponent>(PrimitiveType::Cube);*/


	return body;

}

RigidBody* EntitySetup::CreatePhysicsTest(const Vector3& position)
{
	std::string name = "Test_" + std::to_string(boxCount);
	Entity boxEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Transform& boxTransform = boxEntity.GetComponent<Transform>();



	boxTransform.SetPosition(position);
	boxTransform.SetEularRotation(Vector3(0.0f));
	

	SphereCollider& shape = boxEntity.AddComponent<SphereCollider>();
	shape.Init(BoundingSphere(Vector3(0.0f), 0.5f));

	RigidBody* body = boxEntity.AddComponent<RigidBodyComponent>(boxTransform, shape.GetShape()).GetRigidBody();

	body->SetBounceFactor(0.8f);
	body->SetEntityId(boxEntity);
	ModelComponent& modelComp = boxEntity.AddComponent<ModelComponent>("Crow");

	//return boxEntity;
	return body;

}




SharedPtr<Texture>& EntitySetup::CreateSecurityCamera(const Vector3& position, const Vector3& rotation, const Vector3 camRotation, bool Moving)
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
	Camera* camera = &cameraEntity.AddComponent<Camera>(16.0f / 9.0f, 0.01f, 500.0f);

	cameraEntity.SetParent(meshEntity);

	camera->m_name = "Camera_" + std::to_string(cameraCount);


	//LOG_WARN("Name: {0}", std::to_string((uint64_t)texture.get()));

	return camera->GetRenderTexture();

}

Entity EntitySetup::CreateSkinnedCharatcer(const Vector3& position, const Vector3& scale)
{
	Entity skinnedEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SkinnedMesh");
	Transform& transform = skinnedEntity.GetComponent<Transform>();

	transform.SetPosition(position);
	transform.SetScale(scale);

	SkinnedModelComponent& modelComp = skinnedEntity.AddComponent<SkinnedModelComponent>("RacerCharacter");

	SharedPtr<SkinnedAnimation> idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Idle", "Assets\\Models\\raceridle.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> walkanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Walk", "Assets\\Models\\racerwalk.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> rightanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_RightStrafe", "Assets\\Models\\racerrightwalk.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> leftanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe", "Assets\\Models\\racerleftwalk.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> danceanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe", "Assets\\Models\\racerdance.dae", modelComp.m_handle);

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

RigidBody* EntitySetup::CreatePlayer(const Vector3& position, const Vector3& rotation)
{
	// =============== Base Player ==============
	Entity playerEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerCharacter");
	Transform& playerTransform = playerEntity.GetComponent<Transform>();

	playerTransform.SetPosition(position);
	playerTransform.SetEularRotation(Vector3(0.0f));

	CapsuleCollider& capsuleCol = playerEntity.AddComponent<CapsuleCollider>();
	capsuleCol.Init(BoundingCapsule(1.2f,0.3f,BoundingCapsule::Axis::Y));

	RigidBody* rigidBody = playerEntity.AddComponent<RigidBodyComponent>(playerTransform,capsuleCol.GetShape()).GetRigidBody();
	rigidBody->SetBounceFactor(0.1f);
	rigidBody->SetFriction(0.95f);
	rigidBody->SetEntityId(playerEntity);

	Hierarchy& playerHie = playerEntity.AddComponent<Hierarchy>();
	
	// =============== Player Look ===============
	Entity lookEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerLook");
	Transform& lookTransform = lookEntity.GetComponent<Transform>();

	//Audio::AudioSource* source = 
	Audio::AudioListener* listener = lookEntity.AddComponent<AudioListenerComponent>().GetListener();

	lookTransform.SetPosition(Vector3{0.0f,0.70f,0.0f });
	lookTransform.SetEularRotation(Vector3(0.0f,rotation.y,0.0f));

	Hierarchy& lookHie = lookEntity.AddComponent<Hierarchy>();
	Camera& playerCamera = lookEntity.AddComponent<Camera>(70.0f,0.01f,1000.0f,16.0f/10.0f);

	PlayerLook& look = lookEntity.AddComponent<PlayerLook>();


	// ================ PLayer Hands ================
	Entity skinnedEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PLayerHands");
	Transform& transform = skinnedEntity.GetComponent<Transform>();

	transform.SetPosition(Vector3(0.0f,-1.55f,-0.40f));
	transform.SetEularRotation(Vector3(180.0f,-0.02f,180.0f));
	transform.SetScale(Vector3(0.01f));

	SkinnedModelComponent& modelComp = skinnedEntity.AddComponent<SkinnedModelComponent>("Gun_Pistol");
	AnimatorComponent& animComp = skinnedEntity.AddComponent<AnimatorComponent>();


	SharedPtr<SkinnedAnimation> idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Idle", "Assets\\Models\\GUN_PISTOL_IDLE.fbx", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> reloadanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Reload", "Assets\\Models\\GUN_PISTOL_RELOAD.fbx", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> shootanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Shoot", "Assets\\Models\\GUN_PISTOL_SHOOT.fbx", modelComp.m_handle);
	
	animComp.GetAnimator()->AddAnimation("Idle", idleanim);
	animComp.GetAnimator()->AddAnimation("Reload", reloadanim);
	animComp.GetAnimator()->AddAnimation("Shoot", shootanim);

	animComp.GetAnimator()->PlayAnimation(idleanim);

	skinnedEntity.AddComponent<Hierarchy>();

	skinnedEntity.SetParent(lookEntity);


	SharedPtr<Material>& matOne = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	matOne->SetAlbedoTexture("fppistol");
	matOne->albedoMapFactor = 1.0f;
	//matOne->normalMapFactor = 0.0f;

	SharedPtr<Material>& matTwo = modelComp.m_handle->GetMeshes()[2]->GetMaterial();
	matTwo->SetAlbedoTexture("fppistol");
	matTwo->albedoMapFactor = 1.0f;

	SharedPtr<Material>& matThree = modelComp.m_handle->GetMeshes()[3]->GetMaterial();
	matThree->SetAlbedoTexture("fphand");
	matThree->albedoMapFactor = 1.0f;

	SharedPtr<Material>& matFour = modelComp.m_handle->GetMeshes()[1]->GetMaterial();
	matFour->SetAlbedoTexture("fppistol");
	matOne->albedoMapFactor = 1.0f;
	
	
	
	// ================ Player Bindings ============

	lookEntity.SetParent(playerEntity);

	PlayerCharacter* player = playerEntity.AddComponent<PlayerComponent>().GetPlayer();
	player->SetRigidBodyRef(rigidBody);
	player->SetAnimatorRef(animComp.GetAnimator());
	return rigidBody;

}

void EntitySetup::AssignMaterial(SharedPtr<Material>& mat, const SubwayMaterial type)
{
	switch (type)
	{
	case EntitySetup::SubwayMaterial::MAT_STRUCTURE:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Struct_Diffuse");
		mat->SetNormalTexture("Struct_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;

	case EntitySetup::SubwayMaterial::MAT_STAIRS:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Stairs_Diffuse");
		mat->SetNormalTexture("Stairs_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;
	case EntitySetup::SubwayMaterial::MAT_ADDITION:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Addition_Diffuse");
		mat->SetNormalTexture("Addition_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;
	case EntitySetup::SubwayMaterial::MAT_POSTERS:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Poster_Diffuse");
		mat->SetNormalTexture("Poster_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;
	case EntitySetup::SubwayMaterial::MAT_SUBWAYASSETS:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("SubwayAssets_Diffuse");
		mat->SetNormalTexture("SubwayAssets_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;
	case EntitySetup::SubwayMaterial::MAT_TICKETMACHINE:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Ticketmachine_Diffuse");
		mat->SetNormalTexture("Ticketmachine_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;
		break;
	case EntitySetup::SubwayMaterial::MAT_TUNNEL:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Tunnel_Diffuse");
		mat->SetNormalTexture("Tunnel_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;
		break;
	case EntitySetup::SubwayMaterial::MAT_TURNSTILE:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("Turnstile_Diffuse");
		mat->SetNormalTexture("Turnstile_Normal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;
		break;
	default:
		break;
	}
}



RigidBody* EntitySetup::CreateZombie(const Vector3& position, const Vector3& rotation, std::vector<AudioClip*> clips,ZombieType type)
{

	std::string name = "Zombie_" + std::to_string(enemyCount);
	Entity enemyEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);

	enemyCount++;

	Transform& enemyTransform = enemyEntity.GetComponent<Transform>();

	AudioClip* idleclip = clips[0];
	AudioClip* attackclip = clips[1];
	AudioClip* alertclip = clips[2];
	AudioClip* deathclip = clips[3];


	enemyTransform.SetPosition(position);
	enemyTransform.SetEularRotation(Vector3(0.0f));
	Audio::AudioSource* source = AudioManager::GetInstance().CreateSource(enemyEntity);
	source->SetAudioClip(idleclip);
	source->PlayClipLoop();
	source->Set3DMaxDist(6.0f);
	source->Set3DMinDist(0.1f);
	
	

	CapsuleCollider& capsuleCol = enemyEntity.AddComponent<CapsuleCollider>();

	capsuleCol.Init(BoundingCapsule(1.2f, 0.3f, BoundingCapsule::Axis::Y));

	RigidBody* rigidBody = enemyEntity.AddComponent<RigidBodyComponent>(enemyTransform, capsuleCol.GetShape()).GetRigidBody();
	rigidBody->SetBounceFactor(0.1f);
	rigidBody->SetFriction(0.95f);
	rigidBody->SetEntityId(enemyEntity);
	EnemyCharacter* enemy = enemyEntity.AddComponent<EnemyComponent>().GetEnemy();

	enemy->SetType(type);
	

	Hierarchy& playerHie = enemyEntity.AddComponent<Hierarchy>();

	// =============== Enemy Look ===============
	Entity lookEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("EnemyLook");
	Transform& lookTransform = lookEntity.GetComponent<Transform>();

	lookTransform.SetPosition(Vector3{ 0.0f,-0.9f,0.0f });
	lookTransform.SetEularRotation(Vector3(0.0f, rotation.y, 0.0f));
	lookTransform.SetScale(Vector3(0.01f));
	//lookTransform.SetScale(Vector3(1.0f));

	lookEntity.AddComponent<EnemyLook>();

	Hierarchy& lookHie = lookEntity.AddComponent<Hierarchy>();
	SkinnedModelComponent& modelComp = lookEntity.AddComponent<SkinnedModelComponent>("WarZombie");
	AnimatorComponent& animComp = lookEntity.AddComponent<AnimatorComponent>();


	SharedPtr<SkinnedAnimation> idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Zombie_Idle", "Assets\\Models\\Anims\\Zombie_Idle.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> hitanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Zombie_Hit", "Assets\\Models\\Anims\\Zombie_Hit.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> deathanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Zombie_Death_One", "Assets\\Models\\Anims\\Zombie_Death_1.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> attackAnim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Zombie_Attack", "Assets\\Models\\Anims\\Zombie_Attack.dae", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> walkAnim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Zombie_Walk", "Assets\\Models\\Anims\\Zombie_Walk.dae", modelComp.m_handle);


	animComp.GetAnimator()->AddAnimation("Idle", idleanim);
	animComp.GetAnimator()->AddAnimation("HitOne", hitanim);
	animComp.GetAnimator()->AddAnimation("DeathOne", deathanim); 
	animComp.GetAnimator()->AddAnimation("Walk", walkAnim);
	animComp.GetAnimator()->AddAnimation("Attack", attackAnim);

	animComp.GetAnimator()->PlayAnimation(idleanim);

	enemy->SetAnimatorRef(animComp.GetAnimator());
	enemy->SetRigidBodyRef(rigidBody);


	Audio::AudioSource* screamsource = AudioManager::GetInstance().CreateSource(lookEntity);
	screamsource->SetAudioClip(idleclip);
	screamsource->Set3DMaxDist(6.0f);
	screamsource->Set3DMinDist(0.1f);
	screamsource->SetLoop(false);

	enemy->SetAudioClips(idleclip, alertclip, attackclip, deathclip);
	enemy->SetAudioSource(screamsource,source);
	

	lookEntity.SetParent(enemyEntity);


	SharedPtr<Material>& matOne = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	matOne->SetAlbedoTexture("zombieOneDiffuse");
	matOne->SetNormalTexture("zombieOneNormal");
	matOne->albedoMapFactor = 1.0f;
	matOne->normalMapFactor = 1.0f;

	
	//==== Spawn the Weapon

	Entity weaponEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Weapon_Rifle");
	Transform& weaponTransform = weaponEntity.GetComponent<Transform>();

	weaponTransform.SetPosition(Vector3(0.0f));
	weaponTransform.SetEularRotation(Vector3(0.0f));
	weaponTransform.SetScale(Vector3(1.0f));


	enemy->SetWeaponTransform(&weaponTransform);


	return rigidBody;
}


RigidBody* EntitySetup::CreatePhysicsKinematic(const Vector3& position)
{
	Entity boxEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Kinematic");
	Transform& boxTransform = boxEntity.GetComponent<Transform>();
	boxTransform.SetEularRotation(Vector3(0.0f));


	boxTransform.SetPosition(position);



	SphereCollider& shape = boxEntity.AddComponent<SphereCollider>();
	shape.Init(BoundingSphere(Vector3(0.0f), 0.5f));

	RigidBody* body = boxEntity.AddComponent<RigidBodyComponent>(boxTransform, shape.GetShape()).GetRigidBody();

	body->SetBounceFactor(0.8f);
	body->SetEntityId(boxEntity);

	ModelComponent& modelComp = boxEntity.AddComponent<ModelComponent>("Crow");
	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

	mat->albedoColour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	mat->albedoMapFactor = 0.2f;
	//return boxEntity;
	return body;
}