#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"

static int cameraCount = 0;

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
	STOOL
};


static std::array<std::string, 23> LakeModelString =

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
	"Stool"

};




void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

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

	modelLib->LoadModel("Table", "Assets\\Models\\Lake_Table.fbx");
	modelLib->LoadModel("Stool", "Assets\\Models\\Lake_Stool.fbx");
	modelLib->LoadModel("TV", "Assets\\Models\\TV.fbx");
	modelLib->LoadModel("TVScreen", "Assets\\Models\\TVScreen.fbx");

	modelLib->LoadModel("Cam", "Assets\\Models\\Cam.fbx");
	modelLib->LoadModel("CamBase", "Assets\\Models\\CamBase.fbx");



}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("pallet", "Assets\\Textures\\color.png", TextureFormat::RGBA);
	textureLib->LoadTexture("water", "Assets\\Textures\\water.png", TextureFormat::RGBA);
	textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);
	
	
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
	light.intensity = 0.7f;

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



SharedPtr<Texture> CreateTV(const Vector3& position = Vector3(0.0f), const Vector3& rotation = Vector3(0.0f), SharedPtr<Texture> cameraTex = nullptr ,const DistortType type = DistortType::DISTORT)
{
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
	screentransform.SetPosition(Vector3(0.0f));

	ModelComponent& screenModel = screenentity.AddComponent<ModelComponent>("TVScreen");


	SharedPtr<Material> screenmat = screenModel.m_handle->GetMeshes()[0]->GetMaterial();
	screenmat->textureMaps.albedoMap = cameraTex;
	//screenmat->m_type = MaterialType::Custom;
	//mat->SetMetallicTexture("noise");
	//mat->name = type == DistortType::DISTORT ? "Distort" : "Chromatic";

	screenentity.SetParent(parententity);

	return screenmat->textureMaps.albedoMap;

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



SharedPtr<Texture> CreateSecurityCamera(const Vector3& position = Vector3(0.0f),const Vector3& rotation = Vector3(0.0f), const Vector3 camRotation = Vector3(0.0f))
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
	SecurityCamera& cameraSec = rotatorEntity.AddComponent<SecurityCamera>();
	rottransform.SetPosition(Vector3(0.0f));
	rottransform.SetEularRotation(camRotation);

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
	Camera& camera = cameraEntity.AddComponent<Camera>(16.0f/9.0f,0.01f,1500.0f);

	cameraEntity.SetParent(meshEntity);

	return camera.GetRenderTexture();

}

#endif //  EntitySetup

