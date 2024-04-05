#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"
#include "PlayerCharacter.h"
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

	

	static std::array<std::string, 24> LakeModelString;
	




	static void LoadAllModels();	
	static void LoadAllTextures();



	static Entity CreateMainCamera(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });
	static Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f });	
	static Entity CreateDirectionalLight(const Vector3& rotation = Vector3{ 0.0f });
	 

	static Entity CreateEntity(const LakeModel model, const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const Vector3& scale = Vector3(1.0f));
	static void CreateTV(SharedPtr<Texture>& camtex, const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const DistortType type = DistortType::DISTORT);
	static SharedPtr<Texture>& CreateSecurityCamera(const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), const Vector3 camRotation = Vector3(0.0f), bool Moving = false);

	static Entity CreateSkinnedCharatcer(const Vector3& position = Vector3{ 0.0f }, const Vector3& scale = Vector3{ 1.0f });

	static RigidBody* CreatePhysicsFloor(const Vector3& position, const Vector3& rotation);
	static RigidBody* CreatePhysicsTest(const Vector3& position);
	static RigidBody* CreatePhysicsKinematic(const Vector3& position);
	static RigidBody* CreateZombie(const Vector3& position, const Vector3& rotation, ZombieType type = ZombieType::WALK);



	static RigidBody* CreatePlayer(const Vector3& position, const Vector3& rotation);

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



	
	
};

#endif //  EntitySetup

