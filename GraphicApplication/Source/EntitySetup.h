#ifndef  ENTITYSETUP_H
#define  ENTITYSETUP_H

#include "SaltnPepperEngine.h"
#include "PlayerCharacter.h"
#include "AnimationDefinitions.h"

namespace SaltnPepperEngine 
{
	class EntitySpawner {
	public:
		//static int platformId;
		

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

		static void LoadAllModels()
		{
			SharedPtr<ModelLibrary>& modelLib = Application::GetCurrent().GetModelLibrary();

			modelLib->LoadModel("Cat", "Assets\\Models\\Cat.fbx");
			modelLib->LoadModel("HandLeft", "Assets\\Models\\Hand_Left.fbx");
			modelLib->LoadModel("HandRight", "Assets\\Models\\Hand_Right.fbx");
			modelLib->LoadModel("Sheep", "Assets\\Models\\Sheep.fbx");

		}

		static void LoadAllTextures()
		{
			SharedPtr<TextureLibrary>& textureLib = Application::GetCurrent().GetTextureLibrary();


			textureLib->LoadTexture("cat", "Assets\\Textures\\cat.png", TextureFormat::RGBA);
			textureLib->LoadTexture("grass", "Assets\\Textures\\grass2.png", TextureFormat::RGBA);
			textureLib->LoadTexture("snow", "Assets\\Textures\\snow.png", TextureFormat::RGBA);
			textureLib->LoadTexture("sheep", "Assets\\Textures\\sheep.png", TextureFormat::RGBA);
		}

		static Entity CreateMainCamera(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f })
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

		static Entity CreateParentedEntity(Vector3 Position = Vector3{ 0.0f }, Vector3 Rotation = Vector3{ 0.0f })
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


		static Entity CreatePlayerCharacter(Entity mainCamera, Entity leftHand, Entity rightHand, Vector3 Position = Vector3(0.0f, 3.0f, 0.0f))
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
			properties.isKinematic = false;
			RigidBody3D* bodyRef = playerbaseEntity.AddComponent<RigidBodyComponent>(properties).GetRigidBody().get();
			bodyRef->m_isKinematic = false;
			bodyRef->SetVelocity(Vector3(5.0f, 0.0f, 0.0f));

			Entity childEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("PlayerCamera");
			childEntity.AddComponent<PlayerLook>();
			Hierarchy& hierarchyChildComp = childEntity.AddComponent<Hierarchy>();
			Transform* childTransform = &childEntity.GetComponent<Transform>();
			childTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));

			/*Entity groundDetectorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("GroundDetecttor");
			groundDetectorEntity.AddComponent<PlayerLook>();
			Hierarchy& hierarchyGroundComp = groundDetectorEntity.AddComponent<Hierarchy>();
			Transform* groundTransform = &groundDetectorEntity.GetComponent<Transform>();
			groundTransform->SetRotation(Vector3(0.0f, 0.0f, 0.0f));


			PhysicsProperties propertiesdetector;
			propertiesdetector.collider = MakeShared<SphereCollider>(1.6f);
			propertiesdetector.mass = 20.0f;
			propertiesdetector.friction = 0.8f;
			propertiesdetector.position = Position;
			propertiesdetector.elasticity = 0.1f;
			propertiesdetector.elasticity = 0.1f;
			propertiesdetector.isKinematic = true;
			RigidBody3D* detectorbodyRef = groundDetectorEntity.AddComponent<RigidBodyComponent>(propertiesdetector).GetRigidBody().get();
			bodyRef->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));*/



			//ModelComponent& modelComp = childEntity.AddComponent<ModelComponent>("PlayerKnight");
			//SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

			//mat->SetAlbedoTexture("player.png");

			mainCamera.AddComponent<Hierarchy>();
			leftHand.AddComponent<Hierarchy>();
			rightHand.AddComponent<Hierarchy>();


			childEntity.SetParent(playerbaseEntity);
			//groundDetectorEntity.SetParent(playerbaseEntity);
			mainCamera.SetParent(playerbaseEntity);

			leftHand.SetParent(mainCamera);
			rightHand.SetParent(mainCamera);

			Transform* cameraTransform = &mainCamera.GetComponent<Transform>();
			AnimationComponent* leftHandAnimator = &leftHand.GetComponent<AnimationComponent>();
			AnimationComponent* rightHandAnimator = &rightHand.GetComponent<AnimationComponent>();


			cameraTransform->SetPosition(Vector3(0.0f, 3.0f, 0.0f));
			player.Init(bodyRef, cameraTransform);
			player.SetAnimators(leftHandAnimator, rightHandAnimator);



			return playerbaseEntity;
		}

		static Entity CreateDirectionalLight(const Vector3& rotation)
		{
			//Vector3(-36.0f, 0.0f, 0.0f)
			Entity dirLightEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Directional Light");
			Transform& transform = dirLightEntity.GetComponent<Transform>();
			transform.SetEularRotation(rotation);
			Light& light = dirLightEntity.AddComponent<Light>();
			light.type = LightType::DirectionLight;
			light.intensity = 1.2f;

			return dirLightEntity;
		}

		static Entity CreateBaseFloor()
		{
			Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Floor");
			Transform& transform = floorEntity.GetComponent<Transform>();

			transform.SetScale(Vector3(100.0f, 1.0f, 100.0f));
			transform.SetPosition(Vector3(0.0f, -0.5f, 0.0f));

			ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
			SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

			PhysicsProperties properties;
			properties.collider = MakeShared<BoxCollider>(Vector3(50.0f, 0.5f, 50.0f));
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


		static Entity CreateHand(bool Left)
		{
			std::string name = Left ? "Left" : "Right";

			Entity handEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Hand_" + name);
			Transform& transform = handEntity.GetComponent<Transform>();

			Vector3 position = Left ? Vector3(-0.7f, -0.5f, 0.0f) : Vector3(0.7f, -0.5f, 0.0f);

			transform.SetPosition(position);

			ModelComponent& modelComp = handEntity.AddComponent<ModelComponent>(Left ? "HandLeft" : "HandRight");
			SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();
			mat->albedoColour = Vector4(0.59f, 0.44f, 0.33f, 1.0f);


			AnimationComponent& animComp = handEntity.AddComponent<AnimationComponent>();

			SharedPtr<Animation>& idleAnim = animComp.AddAnimation("Idle");

			idleAnim->AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), 0.0f, EasingType::Linear);
			idleAnim->AddRotationKey(Vector3(-5.0f, 0.0f, 0.0f), 1.0f, EasingType::Linear);
			idleAnim->AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), 2.0f, EasingType::Linear);
			idleAnim->loop = true;

			SharedPtr<Animation>& walkAnim = animComp.AddAnimation("Walk");

			walkAnim->AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), 0.0f, EasingType::Linear);
			walkAnim->AddRotationKey(Vector3(Left ? -20.0f : 20.0f, 0.0f, 0.0f), 0.3f, EasingType::Linear);
			walkAnim->AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), .6f, EasingType::Linear);
			walkAnim->AddRotationKey(Vector3(Left ? 20.0f : -20.0f, 0.0f, 0.0f), 0.9f, EasingType::Linear);
			walkAnim->AddRotationKey(Vector3(0.0f, 0.0f, 0.0f), 1.2f, EasingType::Linear);
			walkAnim->loop = true;

			/*SharedPtr<Animation>& jumpAnim = animComp.AddAnimation("Jump");
			jumpAnim->AddRotationKey(Vector3(-5.0f, Left? 14.0f:-14.0f, 0.0f), 0.0f, EasingType::Linear);
			jumpAnim->loop = true;*/




			return handEntity;
		}

		static void CreatePlatform(Vector3 position, Vector3 size, const std::string& texture, bool ground = false)
		{
			Entity floorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Platform_");
			Transform& transform = floorEntity.GetComponent<Transform>();

			Vector3 halfSize = Vector3(size.x / 2, size.y / 2, size.z / 2);

			transform.SetScale(size);
			transform.SetPosition(position);


			ModelComponent& modelComp = floorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);
			SharedPtr<Material>& mat = modelComp.m_handle->GetMeshes()[0]->GetMaterial();

			PhysicsProperties properties;
			properties.collider = MakeShared<BoxCollider>(halfSize);
			properties.mass = 2000.0f;
			properties.friction = 0.8f;
			properties.stationary = true;
			properties.isStatic = true;
			properties.position = position;
			properties.tag = ground ? CollisionTag::GROUND : CollisionTag::PLATFORM;

			floorEntity.AddComponent<RigidBodyComponent>(properties);

			mat->SetAlbedoTexture(texture);

			//platformId++;
			//return floorEntity;
		}
	};

	//EntitySpawner::platformId
}

#endif //  EntitySetup

