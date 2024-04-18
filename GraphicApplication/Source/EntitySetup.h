#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"
#include "PlayerCharacter.h"
#include "BuddyCharacter.h"
#include "EnemyCharacter.h"

static int cameraCount = 0;
static int tvCount = 0;
static int boxCount = 0;
static int enemyCount = 0;

class EntitySetup
{




public:
	struct FishSchool
	{
		bool isMoving = true;;
	};

	enum class DistortType : uint8_t
	{
		CHROMATIC,
		DISTORT
	};

	enum class SubwayModel : uint8_t
	{
		TEST,
		PLATFROM,
		PLATFROM_BACKWALL,
		PLATFROM_RIGHTWALLL,
		PLATFROM_LEFTWALLL,
		PLATFROM_PILLAR,

		PLATFROM_BENCH,
		PLATFROM_BIN,
		PLATFROM_STAIR,
		PLATFROM_STAIR_RAIL,
		PLATFROM_STAIR_WALL,
		PLATFROM_STAIR_COVER,
		PLATFORM_PIPE,
		PLATFORM_CEILING,

		TUNNEL_WALL,
		TUNNEL_PILLAR,
		TUNNEL_RAILS,
		TUNNEL_RAILSSIDE,
		TUNNEL_BASE,
		TUNNEL_WIRE,
		TUNNEL_CABLE,
		TUNNEL_ARCH,
		TUNNEL_LIGHTS,
		TUNNEL_PIPE,

		POSTER_ADVERTLEFT,
		POSTER_ADVERTRIGHT,
		POSTER_DINO,
		POSTER_FOOL,
		POSTER_LOST,
		POSTER_WORK,
		POSTER_HOTELLEFT,
		POSTER_HOTELRIGHT,
		POSTER_INFO,
		POSTER_HOLIDAY,


		TICKET_WALL_FRONT,
		TICKET_WALL_BACK,
		TICKET_WALL_LEFT,
		TICKET_WALL_RIGHT,
		TICKET_CEILING,

		TICKET_DOOR,
		TICKET_GATE,
		TICKET_GRILL,
		TICKET_MACHINE,
		TICKET_BLOCKADE,

		
		WALL_TAG,
		PILLAR_TAG,
		STAIR_TAG,
		EXIT_TAG,
		TUBE_LIGHT,
		CRATE,
		CRATE_GROUP,
		CRATE_CLOTH,
		BARREL,
		BARREL_CLOTH,
		WOODPALLET


	};

	enum class SubwayMaterial : uint8_t
	{
		MAT_STRUCTURE,
		MAT_STAIRS,
		MAT_ADDITION,
		MAT_POSTERS,
		MAT_SUBWAYASSETS,
		MAT_TICKETMACHINE,
		MAT_TUNNEL,
		MAT_TURNSTILE,
		MAT_CRATE,
		MAT_BARREL,
		MAT_CRATE_CLOTH,
		MAT_BARREL_CLOTH,
		MAT_WOODPALLET

	};

	

	static std::vector<std::string> SubwayModelString;
	




	static void LoadAllModels();	
	static void LoadAllTextures();
	
	static void LoadAllAudio();


	static Entity CreateMainCamera(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });
	static Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });	
	static Entity CreateDirectionalLight(const Vector3& rotation = Vector3{ 0.0f });
	 

	static Entity CreateStaticEntity(const SubwayModel model, const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const Vector3& scale = Vector3(1.0f));
	static void CreateTV(SharedPtr<Texture>& camtex, const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const DistortType type = DistortType::DISTORT);
	static SharedPtr<Texture>& CreateSecurityCamera(const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const Vector3 camRotation = Vector3(0.0f), bool Moving = false);

	static Entity CreateSkinnedCharatcer(const Vector3& position = Vector3{ 0.0f }, const Vector3& scale = Vector3{ 1.0f });


	static Entity CreateTunnelLights(const Vector3& position, const Vector3& color, float radius, float intensity);

	static RigidBody* CreatePhysicsFloor(const Vector3& position, const Vector3& rotation);
	static RigidBody* CreatePhysicsTest(const Vector3& position);
	static RigidBody* CreatePhysicsKinematic(const Vector3& position);
	static EnemyCharacter* CreateZombie(const Vector3& position, const Vector3& rotation, std::vector<AudioClip*> clips, ZombieType type = ZombieType::WALK);

	static Entity CreatePhysicsBox(const Vector3& position, const Vector3& rotation, const BoundingBox bounds, const EnvironmentTag::Tag Tag = EnvironmentTag::Tag::NONE);
	static Entity CreatePhysicsCylinder(const Vector3& position, const Vector3& rotation, const BoundingCylinder bounds);
	//static Entity CreatePhysicsDynamic(const Vector3& position, const Vector3& rotation,)
	static PlayerCharacter* CreatePlayer(const Vector3& position, const Vector3& rotation);

	static BuddyCharacter* CreateBuddy(const Vector3& position, const Vector3& rotation);

	static Entity CreateWaypointBase(const Vector3& position);
	static Entity CreateWaypointArrow(const Vector3& position, Entity base);

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

	private:

		static void AssignMaterial(SharedPtr<Material>& mat,const SubwayMaterial type);

	
	
};

#endif //  EntitySetup

