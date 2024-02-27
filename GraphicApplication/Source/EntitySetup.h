#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "SecurityCamera.h"
#include "ScifiScreen.h"

static int cameraCount = 0;


void LoadAllModels()
{
	SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

	modelLib->LoadModel("Floor", "Assets\\Models\\Floor.fbx");
	modelLib->LoadModel("Ceiling", "Assets\\Models\\Ceiling.fbx");
	modelLib->LoadModel("Seat", "Assets\\Models\\Prop_Seat.fbx");

	modelLib->LoadModel("Asteroid","Assets\\Models\\Asteroid.fbx");
	modelLib->LoadModel("Asteroid_Industrial","Assets\\Models\\Asteroid_Industrial.fbx");
	modelLib->LoadModel("Asteroid_Other", "Assets\\Models\\Asteroid_Other.fbx");
	modelLib->LoadModel("MiningRig", "Assets\\Models\\Asteroid_MiningRig.fbx");
	modelLib->LoadModel("MiningRig_Other","Assets\\Models\\Asteroid_Other_Mine.fbx");
	modelLib->LoadModel("IndustrialMiningRig","Assets\\Models\\Asteroid_Industrial_Mine.fbx");

	// Consoles
	modelLib->LoadModel("CenterConsole","Assets\\Models\\CenterConsole.fbx");
	modelLib->LoadModel("CornerConsoleRight","Assets\\Models\\CornerConsole_Right.fbx");
	modelLib->LoadModel("CornerConsoleLeft", "Assets\\Models\\CornerConsole_Left.fbx");

	// CenterWindow and Screen
	modelLib->LoadModel("CenterWindow", "Assets\\Models\\CenterWindow.fbx");
	modelLib->LoadModel("CenterScreenLeft", "Assets\\Models\\CenterScreenLeft.fbx");
	modelLib->LoadModel("CenterScreenRight", "Assets\\Models\\CenterScreenRight.fbx");

	modelLib->LoadModel("CornerWindowRight", "Assets\\Models\\CornerWindow_Right.fbx");
	modelLib->LoadModel("CornerScreenRight", "Assets\\Models\\CornerScreen_Right.fbx");

	modelLib->LoadModel("CornerWindowLeft", "Assets\\Models\\CornerWindow_Left.fbx");
	modelLib->LoadModel("CornerScreenLeft", "Assets\\Models\\CornerScreen_Left.fbx");


}

void LoadAllTextures()
{
	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	textureLib->LoadTexture("asteroid", "Assets\\Textures\\asteroid.jpg", TextureFormat::RGBA);
	textureLib->LoadTexture("spaceship", "Assets\\Textures\\spaceship.png", TextureFormat::RGBA);

	SharedPtr<Texture> noise = textureLib->LoadTexture("noise", "Assets\\Textures\\noise.png", TextureFormat::RGBA);
	noise->SetWarping(TextureWraping::REPEAT);

	textureLib->LoadTexture("leftOne", "Assets\\Textures\\leftOne.png", TextureFormat::RGBA);
	textureLib->LoadTexture("leftTwo", "Assets\\Textures\\leftTwo.png", TextureFormat::RGBA);
	textureLib->LoadTexture("leftThree", "Assets\\Textures\\leftThree.png", TextureFormat::RGBA);
	

	textureLib->LoadTexture("rightOne", "Assets\\Textures\\rightOne.png", TextureFormat::RGBA);
	textureLib->LoadTexture("rightTwo", "Assets\\Textures\\rightTwo.png", TextureFormat::RGBA);
	textureLib->LoadTexture("rightThree", "Assets\\Textures\\rightThree.png", TextureFormat::RGBA);

	textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
	textureLib->LoadTexture("metal", "Assets\\Textures\\metal.jpg", TextureFormat::RGBA);

	textureLib->LoadTexture("centerwin", "Assets\\Textures\\centerwin.tga", TextureFormat::RGBA);
	textureLib->LoadTexture("centerwin_f", "Assets\\Textures\\centerwin_F.tga", TextureFormat::RGBA);
	textureLib->LoadTexture("cornerscratch", "Assets\\Textures\\cornerscratch.tga", TextureFormat::RGBA);
	textureLib->LoadTexture("cornerdent", "Assets\\Textures\\cornerdent.tga", TextureFormat::RGBA);
	
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
	transform.SetEularRotation(Vector3(154.0f, -51.0f, -180.0f));
	Light& light = dirLightEntity.AddComponent<Light>();
	light.type = LightType::DirectionLight;
	light.intensity = 1.2f;

	return dirLightEntity;
}

// Asteroid Stuff

Entity CreateMainAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateIndustrialAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Industrial_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid_Industrial");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("MiningRig");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

Entity CreateIndustrialMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Industrial_Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("IndustrialMiningRig");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

Entity CreateSecondaryAsteroid(const Vector3& position = Vector3(0.0f))
{
	Entity asteroidEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Asteroid_Other_Entity");
	Transform& asteroidTransform = asteroidEntity.GetComponent<Transform>();

	asteroidTransform.SetPosition(position);

	Hierarchy& hierarchy = asteroidEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = asteroidEntity.AddComponent<ModelComponent>("Asteroid_Other");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("asteroid");

	return asteroidEntity;
}

Entity CreateSecondaryMiningRig(const Vector3& position = Vector3(0.0f))
{
	Entity miningEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Other_Mining_Entity");
	Transform& miningTransform = miningEntity.GetComponent<Transform>();

	miningTransform.SetPosition(position);

	Hierarchy& hierarchy = miningEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = miningEntity.AddComponent<ModelComponent>("MiningRig_Other");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("metal");

	return miningEntity;
}

// Consoles Stuff

Entity CreateConsoleRoom(const Vector3& position = Vector3(0.0f))
{
	Entity consoleRoomEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("ConsoleRoom");
	Hierarchy& hierarchy = consoleRoomEntity.AddComponent<Hierarchy>();

	Transform& roomTransform = consoleRoomEntity.GetComponent<Transform>();
	roomTransform.SetPosition(position);

	return consoleRoomEntity;
}

Entity CreateCenterConsole (const Vector3& position = Vector3(0.0f))
{
	Entity consoleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CenterConsole");
	Transform& consoleTransform = consoleEntity.GetComponent<Transform>();

	consoleTransform.SetPosition(position);

	Hierarchy& hierarchy = consoleEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = consoleEntity.AddComponent<ModelComponent>("CenterConsole");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("spaceship");

	return consoleEntity;
}

Entity CreateCornerConsoleRight(const Vector3& position = Vector3(0.0f))
{
	Entity consoleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CornerConsoleRight");
	Transform& consoleTransform = consoleEntity.GetComponent<Transform>();

	consoleTransform.SetPosition(position);

	Hierarchy& hierarchy = consoleEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = consoleEntity.AddComponent<ModelComponent>("CornerConsoleRight");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("spaceship");

	return consoleEntity;
}

Entity CreateCornerConsoleLeft(const Vector3& position = Vector3(0.0f))
{
	Entity consoleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CornerConsoleLeft");
	Transform& consoleTransform = consoleEntity.GetComponent<Transform>();

	consoleTransform.SetPosition(position);

	Hierarchy& hierarchy = consoleEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = consoleEntity.AddComponent<ModelComponent>("CornerConsoleLeft");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("spaceship");

	return consoleEntity;
}

Entity CreateCenterWindow(const Vector3& position = Vector3(0.0f))
{
	Entity windowEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("CenterWindow");
	Transform& windowTransform = windowEntity.GetComponent<Transform>();

	windowTransform.SetPosition(position);

	Hierarchy& hierarchy = windowEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = windowEntity.AddComponent<ModelComponent>("CenterWindow");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("centerwin");
	mat->SetMetallicTexture("centerwin_f");
	mat->m_type = MaterialType::Transparent;

	return windowEntity;
}

Entity CreateCornerWindow(const Vector3& position = Vector3(0.0f), bool left = false)
{
	Entity windowEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(left? "LeftCornerWindow" : "RightCornerWindow");
	Transform& windowTransform = windowEntity.GetComponent<Transform>();

	windowTransform.SetPosition(position);

	Hierarchy& hierarchy = windowEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = windowEntity.AddComponent<ModelComponent>(left ? "CornerWindowLeft" : "CornerWindowRight");

	SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture(left? "cornerscratch" : "cornerdent");
	mat->SetMetallicTexture("centerwin_f");
	mat->m_type = MaterialType::Transparent;

	return windowEntity;
}

 void CreateFloorCeiling(Entity parent, const Vector3& position)
{
	

	Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Floor");
	Transform& floorTransform = floorEntity.GetComponent<Transform>();

	floorTransform.SetPosition(position);

	Hierarchy& hierarchy = floorEntity.AddComponent<Hierarchy>();

	ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>("Floor");
	SharedPtr<Material> mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("spaceship");

	floorEntity.SetParent(parent);

	Entity ceilingEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Ceiling");
	Transform& cielingTransform = ceilingEntity.GetComponent<Transform>();

	cielingTransform.SetPosition(position + Vector3(0.0f,5.1f,0.0f));

	Hierarchy& hierarchyC = ceilingEntity.AddComponent<Hierarchy>();

	ModelComponent& modelCompC = ceilingEntity.AddComponent<ModelComponent>("Ceiling");
	mat = modelCompC.m_handle->GetMeshes()[0]->GetMaterial();
	mat->SetAlbedoTexture("spaceship");

	ceilingEntity.SetParent(parent);

}

 void CreateWalls(Entity parent)
 {


	 Entity wallOne = Application::GetCurrent().GetCurrentScene()->CreateEntity("WallOne");
	 Transform& wallOneTransform = wallOne.GetComponent<Transform>();

	 wallOneTransform.SetPosition(Vector3(-8.0f,2.53,6.54f));
	 wallOneTransform.SetScale(Vector3(1.0f,8.0f,5.0f));

	 Hierarchy& hierarchy = wallOne.AddComponent<Hierarchy>();

	 ModelComponent& modelComp = wallOne.AddComponent<ModelComponent>(PrimitiveType::Cube);
	 

	 wallOne.SetParent(parent);

	 Entity wallTwo = Application::GetCurrent().GetCurrentScene()->CreateEntity("WallTwo");
	 Transform& wallTwoTransform = wallTwo.GetComponent<Transform>();
	 wallTwoTransform.SetScale(Vector3(1.0f, 8.0f, 5.0f));
	 wallTwoTransform.SetPosition(Vector3(8.0f,2.53f,6.54));

	 Hierarchy& hierarchyC = wallTwo.AddComponent<Hierarchy>();

	 ModelComponent& modelCompC = wallTwo.AddComponent<ModelComponent>(PrimitiveType::Cube);
	

	 wallTwo.SetParent(parent);

 }

 Entity CreateSeat(const Vector3& position)
 {
	 Entity seatEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Seat");
	 Transform& seatTransform = seatEntity.GetComponent<Transform>();

	 seatTransform.SetPosition(position);

	 Hierarchy& hierarchy = seatEntity.AddComponent<Hierarchy>();

	 ModelComponent& modelComp = seatEntity.AddComponent<ModelComponent>("Seat");
	 SharedPtr<Material> mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
	 mat->SetAlbedoTexture("spaceship");

	 return seatEntity;
 }

//SharedPtr<Material> CreateMonitor(const Vector3& position = Vector3(0.0f))
//{
//	Entity tvEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("TV");
//	Hierarchy& hierarchyComp = tvEntity.AddComponent<Hierarchy>();
//	Transform& transform = tvEntity.GetComponent<Transform>();
//
//	transform.SetPosition(position);
//
//	ModelComponent* modelComp = &tvEntity.AddComponent<ModelComponent>(PrimitiveType::Quad);
//	
//	SharedPtr<Material> mat  = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
//	
//	return mat;
//}

//Camera* CreateSecurityCamera(const Vector3& position)
//{
//	Entity meshEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SecuritytCamera");
//	Hierarchy& hierarchyComp = meshEntity.AddComponent<Hierarchy>();
//	Transform& transform = meshEntity.GetComponent<Transform>();
//
//	transform.SetPosition(position);
//
//	ModelComponent* modelComp = &meshEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
//
//
//	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("SecondaryCamera");
//	Hierarchy& camhierarchyComp = cameraEntity.AddComponent<Hierarchy>();
//    Transform& camtransform = cameraEntity.GetComponent<Transform>();
//	camtransform.SetPosition(Vector3(0.0f,1.0f,-2.0f));
//	Camera* camera = &cameraEntity.AddComponent<Camera>();
//
//	cameraEntity.SetParent(meshEntity);
//
//	return camera;
//
//}

Entity CreateCenterScreen(const Vector3& position = Vector3(0.0f), bool isRightScreen = true)
{
	std::string name = isRightScreen ? "CenterScreen_Right" : "CenterScreen_Left";
	Entity screenEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = screenEntity.AddComponent<Hierarchy>();
	Transform& transform = screenEntity.GetComponent<Transform>();
	transform.SetPosition(position);

	float switchtime = isRightScreen ? 4.0f : 5.1f;

	ScifiScreen& screen = screenEntity.AddComponent<ScifiScreen>(switchtime);

	
	std::string modelName = isRightScreen ? "CenterScreenRight" : "CenterScreenLeft";
	ModelComponent* modelComp = &screenEntity.AddComponent<ModelComponent>(modelName);

	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();
	mat->m_type = MaterialType::Custom;

	
	isRightScreen ? mat->SetAlbedoTexture("rightOne") : mat->SetAlbedoTexture("leftOne");
	mat->SetMetallicTexture("noise");
	mat->name = isRightScreen ? "Distort" : "Chromatic";

	SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();

	screen.SetMaterialRef(mat);
	
	if (isRightScreen)
	{
		screen.AddTexture(textureLib->GetResource("rightOne"));
		screen.AddTexture(textureLib->GetResource("rightTwo"));
		screen.AddTexture(textureLib->GetResource("rightThree"));
	}
	else
	{
		screen.AddTexture(textureLib->GetResource("leftOne"));
		screen.AddTexture(textureLib->GetResource("leftTwo"));
		screen.AddTexture(textureLib->GetResource("leftThree"));
	}

	return screenEntity;
}

SharedPtr<Material> CreateCornerScreen(Entity parentEntity,const Vector3& position = Vector3(0.0f), bool isRightScreen = true)
{
	std::string name = isRightScreen ? "CornerScreen_Right" : "CornerScreen_Left";
	Entity screenEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = screenEntity.AddComponent<Hierarchy>();
	Transform& transform = screenEntity.GetComponent<Transform>();
	transform.SetPosition(position);

	screenEntity.SetParent(parentEntity);

	std::string modelName = isRightScreen ? "CornerScreenRight" : "CornerScreenLeft";
	ModelComponent* modelComp = &screenEntity.AddComponent<ModelComponent>(modelName);

	SharedPtr<Material> mat = modelComp->m_handle->GetMeshes()[0]->GetMaterial();

	return mat;
}

SharedPtr<Texture> CreateSecurityCamera(const Vector3& position = Vector3(0.0f),const Vector3& rotation = Vector3(0.0f))
{
	cameraCount++;

	std::string name = "SecurityCameraRotator_" + std::to_string(cameraCount);
	Entity rotatorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& rothierarchyComp = rotatorEntity.AddComponent<Hierarchy>();
	Transform& rottransform = rotatorEntity.GetComponent<Transform>();
	SecurityCamera& cameraSec = rotatorEntity.AddComponent<SecurityCamera>();

	rottransform.SetPosition(position);

	float randomYaw = Random32::Range.GetRandom(-15.0f, 15.0f);
	
	rottransform.SetEularRotation(Vector3(0.0f, randomYaw, 0.0f));

	name = "SecurityCameraEntity_" + std::to_string(cameraCount);
	Entity meshEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& hierarchyComp = meshEntity.AddComponent<Hierarchy>();
	Transform& transform = meshEntity.GetComponent<Transform>();

	transform.SetEularRotation(rotation);

	ModelComponent* modelComp = &meshEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);

	meshEntity.SetParent(rotatorEntity);

	name = "Camera_" + std::to_string(cameraCount);
	Entity cameraEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);
	Hierarchy& camhierarchyComp = cameraEntity.AddComponent<Hierarchy>();
	Transform& camtransform = cameraEntity.GetComponent<Transform>();
	camtransform.SetPosition(Vector3(0.0f, 1.0f, -2.0f));
	Camera& camera = cameraEntity.AddComponent<Camera>(16.0f/9.0f,0.01f,1500.0f);

	cameraEntity.SetParent(meshEntity);

	return camera.GetRenderTexture();

}

#endif //  EntitySetup

