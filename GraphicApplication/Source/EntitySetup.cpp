#include "EntitySetup.h"



std::vector<std::string> EntitySetup::SubwayModelString = {
	
	"SUB_TEST",
	"SUB_PLATFORM",
	"SUB_PLAT_BACKWALL",
	"SUB_PLAT_RIGHTWALL",
	"SUB_PLAT_LEFTWALL",
	"SUB_PLAT_PILLAR",

	"SUB_PLAT_BENCH",
	"SUB_PLAT_BIN",
	"SUB_PLAT_STAIR",
	"SUB_PLAT_STAIR_RAIL",
	"SUB_PLAT_STAIR_WALL",
	"PLATFROM_STAIR_COVER",
	"SUB_PLAT_PIPE",
	"SUB_PLAT_CEILING",

	"SUB_TUNNEL_WALL",
	"SUB_TUNNEL_PILLAR",
	"SUB_TUNNEL_RAIL",
	"SUB_TUNNEL_RAILSIDE",
	"SUB_TUNNEL_BASE",
	"SUB_TUNNEL_WIRE",
	"SUB_TUNNEL_CABLE",
	"SUB_TUNNEL_ARCH",
	"SUB_TUNNEL_LIGHTS",
	"SUB_TUNNEL_PIPE",

	"POSTER_ADVERTLEFT",
	"POSTER_ADVERTRIGHT",
	"POSTER_DINO",
	"POSTER_FOOL",
	"POSTER_LOST",
	"POSTER_WORK",
	"POSTER_HOTELLEFT",
	"POSTER_HOTELRIGHT",
	"POSTER_INFO",
	"POSTER_HOLIDAY",

	"TICKET_WALL_FRONT",
	"TICKET_WALL_BACK",
	"TICKET_WALL_LEFT",
	"TICKET_WALL_RIGHT",
	"TICKET_CEILING",

	"TICKET_DOOR",
	"TICKET_GATE",
	"TICKET_GRILL",
	"TICKET_MACHINE",
	"TICKET_BLOCKADE",
	"TICKET_DISPENSER",

	"WALL_TAG",
	"PILLAR_TAG",
	"STAIR_TAG",
	"EXIT_TAG",

	"TUBE_LIGHT",

	"CRATE",
	"CRATE_GROUP",
	"CRATE_CLOTH",
	"BARREL",
	"BARREL_CLOTH",
	"WOODPALLET",

	"BARRELGROUPONE",
	"CRATEGROUPONE",
	"PALLETGROUPONE",
	"EXITSTAIR"
	
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
	skinnedmodelLib->LoadModel("BuddyBot", "Assets\\Models\\Buddy\\RoboBuddy.dae");
	//skinnedmodelLib->LoadModel("BuddyBot", "Assets\\Models\\Buddy\\RoboBuddy.fbx");


	// Static Meshes


	modelLib->LoadModel("SUB_TEST", "Assets\\Models\\Subway_Test.fbx");
	modelLib->LoadModel("SUB_PLATFORM", "Assets\\Models\\Subway_Platform.fbx");
	modelLib->LoadModel("SUB_PLAT_BACKWALL", "Assets\\Models\\Subway_Platform_BackWall.fbx");
	modelLib->LoadModel("SUB_PLAT_RIGHTWALL", "Assets\\Models\\Subway_Platform_WallRight.fbx");
	modelLib->LoadModel("SUB_PLAT_LEFTWALL", "Assets\\Models\\Subway_Platform_WallLeft.fbx");
	modelLib->LoadModel("SUB_PLAT_PILLAR", "Assets\\Models\\Subway_Pillar.fbx");

	modelLib->LoadModel("SUB_PLAT_BENCH", "Assets\\Models\\Subway_Platform_Bench.fbx");
	modelLib->LoadModel("SUB_PLAT_BIN", "Assets\\Models\\Subway_Platform_Bin.fbx");
	modelLib->LoadModel("SUB_PLAT_STAIR", "Assets\\Models\\Subway_Platform_Stairs.fbx");
	modelLib->LoadModel("SUB_PLAT_STAIR_RAIL", "Assets\\Models\\Subway_Platform_Stairs_Rail.fbx");
	modelLib->LoadModel("SUB_PLAT_STAIR_WALL", "Assets\\Models\\Subway_Platform_Stairs_Wall.fbx");
	modelLib->LoadModel("PLATFROM_STAIR_COVER", "Assets\\Models\\Subway_Stairs_Cover.fbx");
	modelLib->LoadModel("SUB_PLAT_PIPE", "Assets\\Models\\Subway_Platform_Pipe.fbx");
	modelLib->LoadModel("SUB_PLAT_CEILING", "Assets\\Models\\Subway_Ceiling.fbx");

	modelLib->LoadModel("SUB_TUNNEL_WALL", "Assets\\Models\\Subway_Tunnel_Wall.fbx");
	modelLib->LoadModel("SUB_TUNNEL_PILLAR", "Assets\\Models\\Subway_Tunnel_Pillar.fbx");
	modelLib->LoadModel("SUB_TUNNEL_RAIL", "Assets\\Models\\Subway_Tunnel_Rails.fbx");
	modelLib->LoadModel("SUB_TUNNEL_RAILSIDE", "Assets\\Models\\Subway_Tunnel_Rail_Side.fbx");
	modelLib->LoadModel("SUB_TUNNEL_BASE", "Assets\\Models\\Subway_Tunnel_Base.fbx");
	modelLib->LoadModel("SUB_TUNNEL_WIRE", "Assets\\Models\\Subway_Tunnel_Wire.fbx");
	modelLib->LoadModel("SUB_TUNNEL_CABLE", "Assets\\Models\\Subway_Tunnel_Cable.fbx");
	modelLib->LoadModel("SUB_TUNNEL_ARCH", "Assets\\Models\\Subway_Tunnel_Arch.fbx");
	modelLib->LoadModel("SUB_TUNNEL_PIPE", "Assets\\Models\\Subway_Tunnel_Pipe.fbx");
	modelLib->LoadModel("SUB_TUNNEL_LIGHTS", "Assets\\Models\\Subway_Tunnel_Lights.fbx");

	modelLib->LoadModel("POSTER_ADVERTLEFT", "Assets\\Models\\Subway_Poster_AdLeft.fbx");
	modelLib->LoadModel("POSTER_ADVERTRIGHT", "Assets\\Models\\Subway_Poster_AdRight.fbx");
	modelLib->LoadModel("POSTER_DINO", "Assets\\Models\\Subway_Poster_Dino.fbx");
	modelLib->LoadModel("POSTER_FOOL", "Assets\\Models\\Subway_Poster_Fool.fbx");
	modelLib->LoadModel("POSTER_LOST", "Assets\\Models\\Subway_Poster_Lost.fbx");
	modelLib->LoadModel("POSTER_WORK", "Assets\\Models\\Subway_Poster_Work.fbx");

	modelLib->LoadModel("POSTER_HOTELLEFT", "Assets\\Models\\Subway_Poster_HotelLeft.fbx");
	modelLib->LoadModel("POSTER_HOTELRIGHT", "Assets\\Models\\Subway_Poster_HomeRight.fbx");
	modelLib->LoadModel("POSTER_INFO", "Assets\\Models\\Subway_Poster_Info.fbx");
	modelLib->LoadModel("POSTER_HOLIDAY", "Assets\\Models\\Subway_Poster_Holiday.fbx");


	modelLib->LoadModel("TICKET_WALL_FRONT", "Assets\\Models\\Ticket_Wall_Front.fbx");
	modelLib->LoadModel("TICKET_WALL_BACK", "Assets\\Models\\Ticket_Wall_Back.fbx");
	modelLib->LoadModel("TICKET_WALL_LEFT", "Assets\\Models\\Ticket_Wall_Left.fbx");
	modelLib->LoadModel("TICKET_WALL_RIGHT", "Assets\\Models\\Ticket_Wall_Right.fbx");
	modelLib->LoadModel("TICKET_CEILING", "Assets\\Models\\Ticket_Ceiling.fbx");


	modelLib->LoadModel("TICKET_DOOR", "Assets\\Models\\Ticket_Door.fbx");
	modelLib->LoadModel("TICKET_GATE", "Assets\\Models\\Ticket_Gate.fbx");
	modelLib->LoadModel("TICKET_GRILL", "Assets\\Models\\Ticket_Grill.fbx");
	modelLib->LoadModel("TICKET_MACHINE", "Assets\\Models\\Ticket_Machine.fbx");
	modelLib->LoadModel("TICKET_BLOCKADE", "Assets\\Models\\Ticket_Blockade.fbx");
	modelLib->LoadModel("TICKET_DISPENSER", "Assets\\Models\\TicketDispenser.fbx");

	modelLib->LoadModel("WALL_TAG", "Assets\\Models\\Wall_Tag.fbx");
	modelLib->LoadModel("PILLAR_TAG", "Assets\\Models\\Pillar_Tag.fbx");
	modelLib->LoadModel("STAIR_TAG", "Assets\\Models\\Stair_Tag.fbx");
	modelLib->LoadModel("EXIT_TAG", "Assets\\Models\\Exit_Tag.fbx");
	modelLib->LoadModel("TUBE_LIGHT", "Assets\\Models\\Subway_TubeLight.fbx");
	modelLib->LoadModel("CRATE", "Assets\\Models\\Crate.fbx");
	modelLib->LoadModel("CRATE_GROUP", "Assets\\Models\\CrateGroup.fbx");
	modelLib->LoadModel("CRATE_CLOTH", "Assets\\Models\\CrateCloth.fbx");
	modelLib->LoadModel("WOODPALLET", "Assets\\Models\\WoodPallet.fbx");
	modelLib->LoadModel("BARREL", "Assets\\Models\\Barrel.fbx");
	modelLib->LoadModel("BARREL_CLOTH", "Assets\\Models\\BarrelCloth.fbx");

	modelLib->LoadModel("BARRELGROUPONE", "Assets\\Models\\BarrelGroupOne.fbx");
	modelLib->LoadModel("CRATEGROUPONE", "Assets\\Models\\CrateGroupOne.fbx");
	modelLib->LoadModel("PALLETGROUPONE", "Assets\\Models\\PalletGroupOne.fbx");
	modelLib->LoadModel("EXITSTAIR", "Assets\\Models\\ExitStair.fbx");


	

	modelLib->LoadModel("NavMesh_Main", "Assets\\Models\\NavMeshProduction.fbx");
	modelLib->LoadModel("Waypoint_Arrow", "Assets\\Models\\WaypointArrow.fbx");
	modelLib->LoadModel("Waypoint_Base", "Assets\\Models\\WaypointBase.fbx");
	modelLib->LoadModel("Waypoint_Skull", "Assets\\Models\\SkullMark.fbx");
	modelLib->LoadModel("Crosshair", "Assets\\Models\\Crosshair.fbx");

	modelLib->LoadModel("MuzzleFlash", "Assets\\Models\\MuzzleFlash.fbx");


}

void EntitySetup::LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	
	textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);

	textureLib->LoadTexture("fphand", "Assets\\Textures\\hand.png", TextureFormat::RGBA);
	textureLib->LoadTexture("fppistol", "Assets\\Textures\\pistol.png", TextureFormat::RGBA);

	textureLib->LoadTexture("zombieOneDiffuse", "Assets\\Textures\\zombie_D.png", TextureFormat::RGB);
	textureLib->LoadTexture("zombieOneNormal", "Assets\\Textures\\zombie_N.png", TextureFormat::RGB);


	// ============================= STRUCTURE TEXTURES ======================
	textureLib->LoadTexture("Struct_Diffuse", "Assets\\Textures\\M_Structure_baseColor.png", TextureFormat::RGB);
	textureLib->LoadTexture("Struct_Normal", "Assets\\Textures\\M_Structure_normal.png", TextureFormat::RGB);

	// ============================= STAIRS TEXTURES ======================
	textureLib->LoadTexture("Stairs_Diffuse", "Assets\\Textures\\M_Stairs_baseColor.png", TextureFormat::RGB);
	textureLib->LoadTexture("Stairs_Normal", "Assets\\Textures\\M_Stairs_normal.png", TextureFormat::RGB);


	// ============================= ADDITION TEXTURES ======================
	textureLib->LoadTexture("Addition_Diffuse", "Assets\\Textures\\M_Addition_baseColor.png", TextureFormat::RGB);
	textureLib->LoadTexture("Addition_Normal", "Assets\\Textures\\M_Addition_normal.png", TextureFormat::RGB);


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

	textureLib->LoadTexture("buddynormal", "Assets\\Textures\\buddy_norm.png", TextureFormat::RGBA);
	textureLib->LoadTexture("buddydiffuse", "Assets\\Textures\\buddy_diffuse.png", TextureFormat::RGBA);

	textureLib->LoadTexture("cratediffuse", "Assets\\Textures\\cratediffuse.png", TextureFormat::RGBA);
	textureLib->LoadTexture("cratenormal", "Assets\\Textures\\cratenormal.png", TextureFormat::RGBA);


	textureLib->LoadTexture("clothcratediffuse", "Assets\\Textures\\clothcratediffuse.tga", TextureFormat::RGBA);
	textureLib->LoadTexture("clothcratenormal", "Assets\\Textures\\clothcratenormal.png", TextureFormat::RGBA);

	textureLib->LoadTexture("woodpalletdiffuse", "Assets\\Textures\\woodpalletdiffuse.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("woodpalletnormal", "Assets\\Textures\\woodpalletnormal.png", TextureFormat::RGBA);

	textureLib->LoadTexture("barreldiffuse", "Assets\\Textures\\barreldiffuse.png", TextureFormat::RGBA);
	textureLib->LoadTexture("barrelnormal", "Assets\\Textures\\barrelnormal.png", TextureFormat::RGBA);


	textureLib->LoadTexture("barrelclothdiffuse", "Assets\\Textures\\barrelclothdiffuse.tga", TextureFormat::RGBA);
	
	textureLib->LoadTexture("barrelclothnormal", "Assets\\Textures\\barrelclothnormal.png", TextureFormat::RGBA);
	textureLib->LoadTexture("muzzleflash", "Assets\\Textures\\MuzzleFlash.tga", TextureFormat::RGBA);

}

void EntitySetup::LoadAllAudio()
{
	SharedPtr<AudioLibrary>& audioLib = Application::GetCurrent().GetAudioLibrary();

	audioLib->LoadAudio("zombieidle1","Assets\\Audio\\zombie_idle_1.mp3");
	audioLib->LoadAudio("zombieattack1","Assets\\Audio\\zombie_attack_1.mp3");
	audioLib->LoadAudio("zombiedeath1","Assets\\Audio\\zombie_dying_1.mp3");
	audioLib->LoadAudio("zombiedeath2","Assets\\Audio\\zombie_dying_2.mp3");
	audioLib->LoadAudio("zombiealert1","Assets\\Audio\\zombie_alert_1.mp3");

	audioLib->LoadAudio("gunshoot","Assets\\Audio\\PistolFire.mp3");
	audioLib->LoadAudio("gunempty","Assets\\Audio\\PistolEmpty.mp3");
	audioLib->LoadAudio("gunreload","Assets\\Audio\\PistolReload.mp3");



	audioLib->LoadAudio("robo_okay","Assets\\Audio\\Robo_Okay.wav");
	audioLib->LoadAudio("robo_moving","Assets\\Audio\\Robo_Moving.wav");
	audioLib->LoadAudio("robo_going","Assets\\Audio\\Robo_Going.wav");

	audioLib->LoadAudio("robo_diecreature","Assets\\Audio\\Robo_DieCreature.wav");
	audioLib->LoadAudio("robo_kill","Assets\\Audio\\Robo_Kill.wav");

	audioLib->LoadAudio("robo_stop","Assets\\Audio\\Robo_Stop.wav");
	audioLib->LoadAudio("robo_dontshoot","Assets\\Audio\\Robo_DontShoot.wav");

	
	


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
	Light* dirlight = dirLightEntity.AddComponent<LightComponent>().GetLightData();
	dirlight->type = LightType::DirectionLight;
	dirlight->intensity = 1.2f;

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
	case SubwayModel::PLATFORM_CEILING: 
	case SubwayModel::TICKET_CEILING:
	case SubwayModel::TICKET_WALL_BACK:
	case SubwayModel::TICKET_WALL_FRONT:
	case SubwayModel::TICKET_WALL_LEFT:
	case SubwayModel::TICKET_WALL_RIGHT:
	case SubwayModel::PLATFROM_STAIR_COVER:
	case SubwayModel::TICKET_DOOR:


		AssignMaterial(mat, SubwayMaterial::MAT_STRUCTURE);
		break;

	case SubwayModel::PLATFROM_PILLAR:
	case SubwayModel::PLATFROM_STAIR:
	case SubwayModel::PLATFROM_STAIR_RAIL:
	case SubwayModel::PLATFROM_STAIR_WALL:
	case SubwayModel::EXIT_TAG:
	case SubwayModel::STAIR_TAG:
	case SubwayModel::PILLAR_TAG:
	case SubwayModel::TUBE_LIGHT:
	case SubwayModel::EXITSTAIR:

		AssignMaterial(mat, SubwayMaterial::MAT_STAIRS);

		break;


	case SubwayModel::PLATFROM_BENCH:
	case SubwayModel::PLATFROM_BIN:
	case SubwayModel::PLATFORM_PIPE:

		AssignMaterial(mat, SubwayMaterial::MAT_SUBWAYASSETS);

		break;

	case SubwayModel::TUNNEL_WALL:
	case SubwayModel::TUNNEL_PILLAR:
	case SubwayModel::TUNNEL_RAILS:
	case SubwayModel::TUNNEL_WIRE:
	case SubwayModel::TUNNEL_CABLE:
	case SubwayModel::TUNNEL_BASE:
	case SubwayModel::TUNNEL_ARCH:
	case SubwayModel::TUNNEL_LIGHTS:
	case SubwayModel::TUNNEL_RAILSSIDE:
	case SubwayModel::TUNNEL_PIPE:

		AssignMaterial(mat, SubwayMaterial::MAT_TUNNEL);

		break;
	case SubwayModel::POSTER_ADVERTLEFT:
	case SubwayModel::POSTER_ADVERTRIGHT:
	case SubwayModel::POSTER_DINO:
	case SubwayModel::POSTER_FOOL:
	case SubwayModel::POSTER_HOLIDAY:
	case SubwayModel::POSTER_HOTELLEFT:
	case SubwayModel::POSTER_HOTELRIGHT:
	case SubwayModel::POSTER_LOST:
	case SubwayModel::POSTER_INFO:
	case SubwayModel::POSTER_WORK:
	case SubwayModel::WALL_TAG:

		AssignMaterial(mat, SubwayMaterial::MAT_POSTERS);
		break;


	case SubwayModel::TICKET_GRILL:

		AssignMaterial(mat, SubwayMaterial::MAT_ADDITION);
		break;

	case SubwayModel::TICKET_MACHINE:

		AssignMaterial(mat, SubwayMaterial::MAT_TURNSTILE);
		break;

	case SubwayModel::TICKET_DISPENSER:
		AssignMaterial(mat, SubwayMaterial::MAT_TICKETMACHINE);
		break;

	case SubwayModel::CRATE:
	case SubwayModel::CRATE_GROUP:
	case SubwayModel::CRATEGROUPONE:

		AssignMaterial(mat, SubwayMaterial::MAT_CRATE);

		break;

	case SubwayModel::CRATE_CLOTH:

		AssignMaterial(mat, SubwayMaterial::MAT_CRATE_CLOTH);
		break;

	case SubwayModel::WOODPALLET:
	case SubwayModel::PALLETGROUPONE:
		AssignMaterial(mat, SubwayMaterial::MAT_WOODPALLET);
		break;

	case SubwayModel::BARREL:
	case SubwayModel::BARRELGROUPONE:

		AssignMaterial(mat, SubwayMaterial::MAT_BARREL);
		break;

	case SubwayModel::BARREL_CLOTH:
		AssignMaterial(mat, SubwayMaterial::MAT_BARREL_CLOTH);

		break;

	}
	


	


	return entity;
}

Entity EntitySetup::CreateTunnelLights(const Vector3& position, const Vector3& color, float radius, float intensity)
{
	Entity tunnelLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Tunnel Light");
	Transform& transform = tunnelLightEntity.GetComponent<Transform>();
	transform.SetEularRotation(Vector3(0.0f));
	transform.SetPosition(position);


	Light* light = tunnelLightEntity.AddComponent<LightComponent>().GetLightData();
	light->type = LightType::PointLight;
	light->intensity = intensity;
	light->color = color;
	light->radius = radius;

	return tunnelLightEntity;


	return Entity();
}

RigidBody* EntitySetup::CreatePhysicsFloor(const Vector3& position, const Vector3& rotation)
{
	Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Physics_Floor");
	Transform& floorTransform = floorEntity.GetComponent<Transform>();



	floorTransform.SetPosition(position);
	floorTransform.SetEularRotation(rotation);

	//ModelComponen& model = floorEntity.AddComponent<ModelComponent>

	BoundingBox bounds{ Vector3(-4.0f,-0.5f,-40.0f),Vector3(4.0f,0.5f,40.0f) };
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

	SkinnedAnimation* idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Idle", "Assets\\Models\\raceridle.dae", modelComp.m_handle).get();
	SkinnedAnimation* walkanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_Walk", "Assets\\Models\\racerwalk.dae", modelComp.m_handle).get();
	SkinnedAnimation* rightanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_RightStrafe", "Assets\\Models\\racerrightwalk.dae", modelComp.m_handle).get();
	SkinnedAnimation* leftanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe", "Assets\\Models\\racerleftwalk.dae", modelComp.m_handle).get();
	SkinnedAnimation* danceanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Racer_LeftStrafe", "Assets\\Models\\racerdance.dae", modelComp.m_handle).get();

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

PlayerCharacter* EntitySetup::CreatePlayer(const Vector3& position, const Vector3& rotation)
{
	// =============== Base Player ==============
	Entity playerEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerCharacter");
	Transform& playerTransform = playerEntity.GetComponent<Transform>();

	playerTransform.SetPosition(position);
	playerTransform.SetEularRotation(Vector3(0.0f));

	CapsuleCollider& capsuleCol = playerEntity.AddComponent<CapsuleCollider>();
	capsuleCol.Init(BoundingCapsule(1.2f, 0.3f, BoundingCapsule::Axis::Y));

	RigidBody* rigidBody = playerEntity.AddComponent<RigidBodyComponent>(playerTransform, capsuleCol.GetShape()).GetRigidBody();
	rigidBody->SetBounceFactor(0.1f);
	rigidBody->SetFriction(0.95f);
	rigidBody->SetEntityId(playerEntity);

	Hierarchy& playerHie = playerEntity.AddComponent<Hierarchy>();

	// =============== Player Look ===============
	Entity lookEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerLook");
	Transform& lookTransform = lookEntity.GetComponent<Transform>();

	//Audio::AudioSource* source = 
	Audio::AudioListener* listener = lookEntity.AddComponent<AudioListenerComponent>().GetListener();

	lookTransform.SetPosition(Vector3{ 0.0f,0.70f,0.0f });
	lookTransform.SetEularRotation(Vector3(0.0f, rotation.y, 0.0f));

	Hierarchy& lookHie = lookEntity.AddComponent<Hierarchy>();
	Camera& playerCamera = lookEntity.AddComponent<Camera>(70.0f, 0.01f, 1000.0f, 16.0f / 9.0f);

	PlayerLook& look = lookEntity.AddComponent<PlayerLook>();

	

	// ================ PLayer Hands ================
	Entity skinnedEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PLayerHands");
	Transform& transform = skinnedEntity.GetComponent<Transform>();

	transform.SetPosition(Vector3(0.0f, -1.55f, -0.40f));
	transform.SetEularRotation(Vector3(180.0f, -0.02f, 180.0f));
	transform.SetScale(Vector3(0.01f));

	SkinnedModelComponent& modelComp = skinnedEntity.AddComponent<SkinnedModelComponent>("Gun_Pistol");
	AnimatorComponent& animComp = skinnedEntity.AddComponent<AnimatorComponent>();


	SkinnedAnimation* idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Idle", "Assets\\Models\\GUN_PISTOL_IDLE.fbx", modelComp.m_handle).get();
	SkinnedAnimation* reloadanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Reload", "Assets\\Models\\GUN_PISTOL_RELOAD.fbx", modelComp.m_handle).get();
	SkinnedAnimation* shootanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Shoot", "Assets\\Models\\GUN_PISTOL_SHOOT.fbx", modelComp.m_handle).get();

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


	// ======== Crosshair Entity ==========

	Entity crossEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Crosshair");
	Transform& crossTransform = crossEntity.GetComponent<Transform>();

	crossTransform.SetPosition(Vector3{ 0.0f,0.0f,-0.1f });
	crossTransform.SetEularRotation(Vector3{ 0.0f });
	crossTransform.SetScale(Vector3{ 0.05f });

	ModelComponent& crossmodel = crossEntity.AddComponent<ModelComponent>("Crosshair");
	SharedPtr<Material>& crossmat = crossmodel.m_handle->GetMeshes()[0]->GetMaterial();
	LightComponent& pointLight = crossEntity.AddComponent<LightComponent>();
	Light* light = pointLight.GetLightData();

	light->color = Vector3{ 1.0f,0.6f,0.0f};
	light->type = LightType::PointLight;
	light->intensity = 0.0f;
	light->radius = 6.0f;

	crossmat->albedoColour = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	crossmat->albedoMapFactor = 0.0f;

	Hierarchy& crosshie = crossEntity.AddComponent<Hierarchy>();
	crossEntity.SetParent(lookEntity);

	// MuzzleFlash Entity
	Entity muzzleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("MuzzleFlash");
	Transform& muzzleTransform = muzzleEntity.GetComponent<Transform>();

	muzzleTransform.SetPosition(Vector3{ 0.1041f,-0.0332f,-0.6194f });
	muzzleTransform.SetEularRotation(Vector3{ 0.0f,0.0f,-12.0f });
	muzzleTransform.SetScale(Vector3{ 0.16f });

	ModelComponent& muzzlemodel = muzzleEntity.AddComponent<ModelComponent>("MuzzleFlash");
	SharedPtr<Material>& muzzlemat = muzzlemodel.m_handle->GetMeshes()[0]->GetMaterial();

	muzzlemat->SetAlbedoTexture("muzzleflash");
	muzzlemat->albedoMapFactor = 1.0f;

	Hierarchy& muzzleshie = muzzleEntity.AddComponent<Hierarchy>();
	muzzleEntity.SetParent(lookEntity);

	// ================ Player Bindings ============

	lookEntity.SetParent(playerEntity);

	Audio::AudioSource* gunsource = AudioManager::GetInstance().CreateSource(playerEntity,false);
	Audio::AudioSource* footsource = AudioManager::GetInstance().CreateSource(playerEntity);

	


	PlayerCharacter* player = playerEntity.AddComponent<PlayerComponent>().GetPlayer();
	player->SetRigidBodyRef(rigidBody);
	player->SetAnimatorRef(animComp.GetAnimator());

	player->SetAudioSources(footsource, gunsource);
	player->SetMuzzleLight(light);
	player->SetMuzzleFlash(muzzleEntity);

	return player;

}

Entity EntitySetup::CreateWaypointArrow(const Vector3& position, Entity base)
{
	Entity arrowEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("MarkerArrow");
	

	Hierarchy& arrowHierarchy = arrowEntity.AddComponent<Hierarchy>();

	ModelComponent& arrowModel = arrowEntity.AddComponent<ModelComponent>("Waypoint_Arrow");

	arrowEntity.SetParent(base);

	Transform& arrowTransform = arrowEntity.GetComponent<Transform>();

	arrowTransform.SetPosition(position);
	arrowTransform.SetEularRotation(Vector3(0.0f));

	SharedPtr<Material>& mat = arrowModel.m_handle->GetMeshes()[0]->GetMaterial();

	mat->albedoMapFactor = 0.0f;
	mat->albedoColour = Vector4{ 0.0f,1.0f,0.0f,1.0f };
	mat->m_type = MaterialType::Opaque;

	return arrowEntity;
}

Entity EntitySetup::CreateWaypointBase(const Vector3& position)
{
	Entity baseEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("MarkerBase");

	Transform& baseTransform = baseEntity.GetComponent<Transform>();

	baseTransform.SetPosition(position);
	baseTransform.SetEularRotation(Vector3(0.0f));

	Hierarchy& baseHierarchy = baseEntity.AddComponent<Hierarchy>();

	ModelComponent& baseModel = baseEntity.AddComponent<ModelComponent>("Waypoint_Base");
	SharedPtr<Material>& mat = baseModel.m_handle->GetMeshes()[0]->GetMaterial();

	mat->albedoMapFactor = 0.0f;
	mat->albedoColour = Vector4{ 0.0f,1.0f,0.0f,1.0f };
	mat->m_type = MaterialType::Opaque;

	return baseEntity;
}

BuddyCharacter* EntitySetup::CreateBuddy(const Vector3& position, const Vector3& rotation)
{
	// =============== Base Player ==============
	Entity buddyEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BuddyCharacter");
	Transform& buddyTransform = buddyEntity.GetComponent<Transform>();

	buddyTransform.SetPosition(position);
	buddyTransform.SetEularRotation(Vector3(0.0f));

	CapsuleCollider& capsuleCol = buddyEntity.AddComponent<CapsuleCollider>();
	capsuleCol.Init(BoundingCapsule(1.2f, 0.3f, BoundingCapsule::Axis::Y));

	RigidBody* rigidBody = buddyEntity.AddComponent<RigidBodyComponent>(buddyTransform, capsuleCol.GetShape()).GetRigidBody();
	rigidBody->SetBounceFactor(0.1f);
	rigidBody->SetFriction(0.95f);
	rigidBody->SetEntityId(buddyEntity);

	EnvironmentTag& tagComp = buddyEntity.AddComponent<EnvironmentTag>();
	tagComp.currentTag = EnvironmentTag::Tag::BUDDY;

	Hierarchy& buddyHierarchy = buddyEntity.AddComponent<Hierarchy>();

	// =============== Buddy Look ===============
	Entity lookEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BuddyLook");
	Transform& lookTransform = lookEntity.GetComponent<Transform>();

	
	lookTransform.SetPosition(Vector3{ 0.0f,-0.890f,0.0f });
	lookTransform.SetEularRotation(Vector3(0.0f, rotation.y, 0.0f));

	Hierarchy& lookHie = lookEntity.AddComponent<Hierarchy>();
	
	BuddyLook& look = lookEntity.AddComponent<BuddyLook>();


	// ================ Buddy Model ================
	Entity skinnedEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("BuddyModel");
	Transform& transform = skinnedEntity.GetComponent<Transform>();

	transform.SetPosition(Vector3(0.0f));
	transform.SetEularRotation(Vector3(0.0f));
	transform.SetScale(Vector3(0.01f));

	SkinnedModelComponent& modelComp = skinnedEntity.AddComponent<SkinnedModelComponent>("BuddyBot");
	AnimatorComponent& animComp = skinnedEntity.AddComponent<AnimatorComponent>();


	SkinnedAnimation* idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Buddy_Idle", "Assets\\Models\\Buddy\\Robo_Idle.dae", modelComp.m_handle).get();
	SkinnedAnimation* walkanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Buddy_Walk", "Assets\\Models\\Buddy\\Robo_Walk.dae", modelComp.m_handle).get();
	SkinnedAnimation* attackanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Buddy_Attack", "Assets\\Models\\Buddy\\Robo_Attack.dae", modelComp.m_handle).get();
	SkinnedAnimation* hitanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Buddy_Hit", "Assets\\Models\\Buddy\\Robo_Hit.dae", modelComp.m_handle).get();
	
	/*SharedPtr<SkinnedAnimation> reloadanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Reload", "Assets\\Models\\GUN_PISTOL_RELOAD.fbx", modelComp.m_handle);
	SharedPtr<SkinnedAnimation> shootanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation("Gun_Pistol_Shoot", "Assets\\Models\\GUN_PISTOL_SHOOT.fbx", modelComp.m_handle);*/

	animComp.GetAnimator()->AddAnimation("Idle", idleanim);
	animComp.GetAnimator()->AddAnimation("Walk", walkanim);
	animComp.GetAnimator()->AddAnimation("Attack", attackanim);
	animComp.GetAnimator()->AddAnimation("Hit", hitanim);


	animComp.GetAnimator()->PlayAnimation(idleanim);

	skinnedEntity.AddComponent<Hierarchy>();

	skinnedEntity.SetParent(lookEntity);


	SharedPtr<Material>& matOne = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	matOne->SetAlbedoTexture("buddydiffuse");
	matOne->SetNormalTexture("buddynormal");
	matOne->albedoMapFactor = 1.0f;
	matOne->normalMapFactor = 0.0f;

	



	// ================ Player Bindings ============

	lookEntity.SetParent(buddyEntity);

	BuddyCharacter* buddy = buddyEntity.AddComponent<BuddyComponent>().GetBuddy();
	buddy->SetRigidBodyRef(rigidBody);
	buddy->SetAnimatorRef(animComp.GetAnimator());


	Audio::AudioSource* speechSource = Audio::AudioManager::GetInstance().CreateSource(buddyEntity);
	Audio::AudioSource* actionSource = Audio::AudioManager::GetInstance().CreateSource(buddyEntity);
	speechSource->Set3DMaxDist(20.0f);
	buddy->SetAudioSource(speechSource,actionSource);
	

	return buddy;

}

Entity EntitySetup::CreatePhysicsBox(const Vector3& position, const Vector3& rotation, const BoundingBox bounds, const EnvironmentTag::Tag Tag)
{
	Entity boxEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Physics_BOX");
	Transform& boxTransform = boxEntity.GetComponent<Transform>();

	boxTransform.SetPosition(position);
	boxTransform.SetEularRotation(rotation);

	EnvironmentTag& tagComp = boxEntity.AddComponent<EnvironmentTag>();
	tagComp.currentTag = Tag;

	BoxCollider& shape = boxEntity.AddComponent<BoxCollider>();
	shape.Init(bounds);
	RigidBody* body = boxEntity.AddComponent<RigidBodyComponent>(boxTransform, shape.GetShape()).GetRigidBody();
	body->MakeStatic();
	body->SetEntityId(boxEntity);
	body->SetBounceFactor(0.5f);
	body->SetFriction(0.8f);

	return boxEntity;
}

Entity EntitySetup::CreatePhysicsCylinder(const Vector3& position, const Vector3& rotation, const BoundingCylinder bounds)
{
	Entity cylinderEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Physics_BOX");
	Transform& cylinderTransform = cylinderEntity.GetComponent<Transform>();

	cylinderTransform.SetPosition(position);
	cylinderTransform.SetEularRotation(rotation);

	CylinderCollider& shape = cylinderEntity.AddComponent<CylinderCollider>();
	shape.Init(bounds);
	RigidBody* body = cylinderEntity.AddComponent<RigidBodyComponent>(cylinderTransform, shape.GetShape()).GetRigidBody();
	body->MakeStatic();
	body->SetEntityId(cylinderEntity);
	body->SetBounceFactor(0.5f);
	body->SetFriction(0.8f);

	return cylinderEntity;
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

	case EntitySetup::SubwayMaterial::MAT_CRATE:
		
		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("cratediffuse");
		mat->SetNormalTexture("cratenormal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;

	case EntitySetup::SubwayMaterial::MAT_CRATE_CLOTH:
		
		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("clothcratediffuse");
		mat->SetNormalTexture("clothcratenormal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;
	
	case EntitySetup::SubwayMaterial::MAT_WOODPALLET:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("woodpalletdiffuse");
		mat->SetNormalTexture("woodpalletnormal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;

	case EntitySetup::SubwayMaterial::MAT_BARREL:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("barreldiffuse");
		mat->SetNormalTexture("barrelnormal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;

	case EntitySetup::SubwayMaterial::MAT_BARREL_CLOTH:

		mat->m_type = MaterialType::Opaque;
		mat->SetAlbedoTexture("barrelclothdiffuse");
		mat->SetNormalTexture("barrelclothnormal");
		mat->normalMapFactor = 1.0f;
		mat->albedoMapFactor = 1.0f;

		break;


	}
}



EnemyCharacter* EntitySetup::CreateZombie(const Vector3& position, const Vector3& rotation, std::vector<AudioClip*> clips,ZombieType type)
{

	std::string name = "Zombie_" + std::to_string(enemyCount);
	Entity enemyEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);


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
	
	EnvironmentTag& tagComp = enemyEntity.AddComponent<EnvironmentTag>();
	tagComp.currentTag = EnvironmentTag::Tag::ENEMY;

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

	const std::string enemyname = "_" + std::to_string(enemyCount);
	const std::string idlename = "Zombie_Idle" + std::string(enemyname);
	const std::string hitname = "Zombie_Hit" + std::string(enemyname);
	const std::string deathname = "Zombie_Death_One" + std::string(enemyname);
	const std::string attackname = "Zombie_Attack" + std::string(enemyname);
	const std::string walkname = "Zombie_Walk" + std::string(enemyname);

	SkinnedAnimation* idleanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation(idlename, "Assets\\Models\\Anims\\Zombie_Idle.dae", modelComp.m_handle).get();
	SkinnedAnimation* hitanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation(hitname , "Assets\\Models\\Anims\\Zombie_Hit.dae", modelComp.m_handle).get();
	SkinnedAnimation* deathanim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation(deathname, "Assets\\Models\\Anims\\Zombie_Death_1.dae", modelComp.m_handle).get();
	SkinnedAnimation* attackAnim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation(attackname, "Assets\\Models\\Anims\\Zombie_Attack.dae", modelComp.m_handle).get();
	SkinnedAnimation* walkAnim = Application::GetCurrent().GetAnimationLibrary()->LoadAnimation(walkname, "Assets\\Models\\Anims\\Zombie_Walk.dae", modelComp.m_handle).get();


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

	
	//==== Spawn the Mark Icon

	Entity markEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Mark_Enemy");
	Transform& markTransform = markEntity.GetComponent<Transform>();

	markTransform.SetPosition(Vector3(0.0f, 1.34f,0.0f));
	markTransform.SetEularRotation(Vector3(0.0f));
	markTransform.SetScale(Vector3(0.6f));


	ModelComponent& markmodle = markEntity.AddComponent<ModelComponent>("Waypoint_Skull");
	SharedPtr<Material>& markMat = markmodle.m_handle->GetMeshes()[0]->GetMaterial();
	markMat->albedoColour = Vector4{ 1.0f,0.0f,0.0f,1.0f };
	markMat->albedoMapFactor = 0.0f;
	Hierarchy& markHie = markEntity.AddComponent<Hierarchy>();
	markEntity.AddComponent<EnemyMark>(); 
	markEntity.SetParent(enemyEntity);
	markEntity.SetActive(false);
	enemy->SetMarkRef(markEntity);

	enemyCount++;

	return enemy;
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