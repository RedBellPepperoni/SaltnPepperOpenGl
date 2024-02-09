#include "Scene.h"
#include "SceneGraph.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/System/Input/InputSystem.h"

// All the Component Includes
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Rendering/Geometry/Model.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Physics/Collision/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/Collision/Colliders/CapsuleCollider.h"
#include "Engine/Core/Rendering/Camera/FlyCameraController.h"


#include "Engine/Core/System/Application/Application.h"
#include "Engine/Utils/Serialization/GLMSerialization.h"

#include <entt/entity/snapshot.hpp>
// Cereal Stuff
#include <cereal/types/memory.hpp>
//#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>




CEREAL_REGISTER_TYPE(SaltnPepperEngine::Physics::SphereCollider);
CEREAL_REGISTER_TYPE(SaltnPepperEngine::Physics::BoxCollider);
CEREAL_REGISTER_TYPE(SaltnPepperEngine::Physics::CapsuleCollider);

CEREAL_REGISTER_POLYMORPHIC_RELATION(SaltnPepperEngine::Physics::Collider, SaltnPepperEngine::Physics::SphereCollider);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SaltnPepperEngine::Physics::Collider, SaltnPepperEngine::Physics::BoxCollider);
CEREAL_REGISTER_POLYMORPHIC_RELATION(SaltnPepperEngine::Physics::Collider, SaltnPepperEngine::Physics::CapsuleCollider);




namespace SaltnPepperEngine
{
	
	using namespace Rendering;
	using namespace Components;
	using namespace Physics;


#define ALL_COMPONENTS Transform, IdComponent ,NameComponent, ActiveComponent, Hierarchy, Camera, ModelComponent, Light, RigidBodyComponent 

#define ALL_ENTTCOMPONENTS(input) get<Transform>(input). \
	get<IdComponent>(input).		\
	get<NameComponent>(input).		\
	get<ActiveComponent>(input).	\
	get<Hierarchy>(input).			\
	get<Camera>(input).				\
    get<Light>(input).				\
	get<RigidBodyComponent>(input). \
	get<ModelComponent>(input)	
	
	Scene::Scene(const std::string& name)
		:m_name(name)
	{
		// Setting up a new Entity Manager with the current scene's Reference
		m_EntityManager = MakeUnique<EntityManager>(this);
		/*m_EntityManager->AddComponentOnConstructDependency<Camera, Transform>();
		m_EntityManager->AddComponentOnConstructDependency<ModelComponent, Transform>();
		m_EntityManager->AddComponentOnConstructDependency<Light, Transform>();*/


		m_SceneGraph = MakeUnique<SceneGraph>();
		m_SceneGraph->Init(GetRegistry());


		

	}

	Scene::~Scene()
	{
		// Cleanup the manager
		m_EntityManager->Clear();
	}

	void Scene::Init()
	{

		//LuaManager::GetInstance().OnInit(this);


		// Setup systems here
	}
	void Scene::CleanUp()
	{


		// Destroy all teh gameobjects in teh registry
		DestroyAllGameObjects();

		//LuaManager::GetInstance().GetState().collect_garbage();

		// Clear other managers if needed
		 
	}
	void Scene::Update(float deltaTime)
	{

		Vector2 mousePosition = Input::InputSystem::GetInstance().GetMousePosition();


		ComponentView cameraControllerView = m_EntityManager->GetComponentsOfType<FlyCameraController>();

		ComponentView cameraview = m_EntityManager->GetComponentsOfType<Camera>();
		////ComponentView audioListenerView = m_EntityManager->GetComponentsOfType<AudioListener>();


		Camera* camera = nullptr;

		if (!cameraview.IsEmpty())
		{
			camera = &cameraview[0].GetComponent<Camera>();
		}


		if (!Application::GetCurrent().GetEditorActive())
		{
			if (!cameraControllerView.IsEmpty())
			{
				FlyCameraController& controller = cameraControllerView[0].GetComponent<FlyCameraController>();
				Transform* transform = cameraControllerView[0].TryGetComponent<Transform>();

				if (transform)
				{

					controller.SetCamera(camera);
					controller.KeyboardInput(*transform, deltaTime);
					controller.MouseInput(*transform, mousePosition, deltaTime);

					Vector3 pos = transform->GetPosition();
					Vector3 rot = transform->GetEulerRotation();

					string fps = std::to_string(Application::GetCurrent().GetFPS());

					std::string position = "Position X : " + std::to_string(pos.x) + " Y : " + std::to_string(pos.y) + " Z : " + std::to_string(pos.z) + "Rotation X : " + std::to_string(rot.x) + " Y : " + std::to_string(rot.y) + " Z : " + std::to_string(rot.z);

					Application::GetCurrent().SetWindowTitle(position);

				}



			}
		}


		
	 	UpdateSceneGraph();


	}
	const std::string& Scene::GetName() const
	{
		return m_name;
	}
	void Scene::SetName(const std::string& name)
	{
	}

	entt::registry& Scene::GetRegistry()
	{
		return m_EntityManager->GetRegistry();
	}

	template <typename T>
	static void CopyComponentIfExists(entt::entity targetEntity, entt::entity sourcelEntity, entt::registry& registry)
	{
		if (registry.all_of<T>(sourcelEntity))
		{
			T& sourceComponent = registry.get<T>(sourcelEntity);
			registry.emplace_or_replace<T>(targetEntity, sourceComponent);
		}
	}

	template <typename... Component>
	static void CopyEntity(entt::entity targetEntity, entt::entity sourcelEntity, entt::registry& registry)
	{
		(CopyComponentIfExists<Component>(targetEntity, sourcelEntity, registry), ...);
	}

	Entity Scene::CreateEntity()
	{
		return m_EntityManager->Create();
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		return m_EntityManager->Create(name);
	}

	void Scene::Duplicate(Entity entity)
	{
		Duplicate(entity, Entity(entt::null, nullptr));
	}

	void Scene::Duplicate(Entity entity, Entity parent)
	{
		m_SceneGraph->DisableOnConstruct(true, m_EntityManager->GetRegistry());

		Entity newEntity = m_EntityManager->Create();

		CopyEntity<ALL_COMPONENTS>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
		newEntity.GetComponent<IdComponent>().ID = UniqueId().GetId();
		//newEntity.GetComponent<RigidBodyComponent>().GetRigidBody()

		Hierarchy* hierarchyComponent = newEntity.TryGetComponent<Hierarchy>();
		if (hierarchyComponent)
		{
			hierarchyComponent->m_First = entt::null;
			hierarchyComponent->m_Parent = entt::null;
			hierarchyComponent->m_Next = entt::null;
			hierarchyComponent->m_Prev = entt::null;
		}

		auto children = entity.GetChildren();
		std::vector<Entity> copiedChildren;

		for (Entity child : children)
		{
			Duplicate(child, newEntity);
		}

		if (parent)
			newEntity.SetParent(parent);

		m_SceneGraph->DisableOnConstruct(false, m_EntityManager->GetRegistry());
	}

	Entity Scene::GetEntitybyId(const uint64_t id)
	{
		return m_EntityManager->GetEntitybyId(id);
	}

	void Scene::DestroyEntity(Entity entity)
	{

		DestroyInternal(entity.GetHandle(), GetRegistry());
		
	}


	void Scene::DestroyInternal(entt::entity entity, entt::registry& registry)
	{
		// Search if the Entity has a hierarchy compoennt and has children
		Hierarchy* hierarchyComponent = registry.try_get<Hierarchy>(entity);

		if (hierarchyComponent)
		{
			// get the First Child entity from the Hierarchy
			entt::entity child = hierarchyComponent->First();

			// Recursively delete all the children
			while (child != entt::null)
			{
				auto hierarchyComponent = registry.try_get<Hierarchy>(child);
				auto next = hierarchyComponent ? hierarchyComponent->Next() : entt::null;
				DestroyInternal(child, registry);
				child = next;
			}

		}

		// Finally Destroy the selected entity
		registry.destroy(entity);
	}


	EntityManager* Scene::GetEntityManager()
	{
		return m_EntityManager.get();
	}
	void Scene::DestroyAllGameObjects()
	{
		// deletes all teh entities in the registry
		m_EntityManager->Clear();
	}


	void Scene::SetMainCamera(CameraController* controller, Transform* transform)
	{
		mainCameraTransform = transform;
		mainCameraController = controller;
		//mainAudioListener = listener;
		//LOG_ERROR("Camera Transform : {0} : {1} : {2}", mainCameraTransform->GetPosition().x, mainCameraTransform->GetPosition().y, mainCameraTransform->GetPosition().z);

	}

	void Scene::SetMainCameraPosition(Vector3 position)
	{
		mainCameraTransform->SetPosition(position);
	}

	void Scene::UpdateSceneGraph()
	{
		m_SceneGraph->Update(m_EntityManager->GetRegistry());
	}






	void Scene::Serialize(const std::string& filename, bool binary)
	{
		LOG_INFO("Scene Saved : [{0}]", filename);

		std::string path = filename;
		path += m_name;

		m_sceneSerializationVersion = SceneSerializarionVersion;

		std::stringstream storage;
		path += std::string(".json");
			
		{
			cereal::JSONOutputArchive output{ storage };
			output(*this);
			entt::snapshot{ m_EntityManager->GetRegistry() }.get<entt::entity>(output).ALL_ENTTCOMPONENTS(output);
		}

		FileSystem::WriteFileToText(path, storage.str());
		
	}

	void Scene::Deserialize(const std::string filename, bool binary)
	{
		m_EntityManager->Clear();
		m_SceneGraph->DisableOnConstruct(true, m_EntityManager->GetRegistry());

		std::string path = filename;
		path += m_name;

		path += std::string(".json");

		if (!FileSystem::Exists(path))
		{
			LOG_ERROR("NO Scene found at path : [{0}]", path);
			return;
		}

		try
		{
			
			std::string data = FileSystem::ReadFileToString(path);
			std::istringstream istr;
			istr.str(data);
			cereal::JSONInputArchive input(istr);
			input(*this);

			entt::registry& reg = m_EntityManager->GetRegistry();

			m_EntityManager->GetRegistry().clear();

			entt::snapshot_loader{ m_EntityManager->GetRegistry() }.get<entt::entity>(input).ALL_ENTTCOMPONENTS(input);


		} 
		catch (...) 
		{ 
			LOG_ERROR("Failed To load Scene : [{0}]", path); 
		} 

		m_SceneGraph->DisableOnConstruct(false, m_EntityManager->GetRegistry());

	}

	Transform* Scene::GetMainCameraTransform() const
	{
		return mainCameraTransform;
	}

	template <typename T>
	static void DeserialiseComponentIfExists(Entity entity, cereal::JSONInputArchive& archive)
	{
		/*bool hasComponent;
		archive(hasComponent);
		if (hasComponent)
			archive(entity.GetOrAddComponent<T>());*/
	}

	template <typename... Component>
	static void DeserialiseEntity(Entity entity, cereal::JSONInputArchive& archive)
	{
		//(DeserialiseComponentIfExists<Component>(entity, archive), ...);
	}


	template <typename T>
	static void SerialiseComponentIfExists(Entity entity, cereal::JSONOutputArchive& archive)
	{
		/*bool hasComponent = entity.HasComponent<T>();
		archive(hasComponent);
		if (hasComponent)
			archive(entity.GetComponent<T>());*/
	}

	template <typename... Component>
	static void SerialiseEntity(Entity entity, cereal::JSONOutputArchive& archive)
	{
		//(SerialiseComponentIfExists<Component>(entity, archive), ...);
	}

	void SerializeEntityHierarchy(Entity entity, cereal::JSONOutputArchive& archive)
	{
		//// Serialize the current entity
		//SerialiseEntity<ALL_COMPONENTS>(entity, archive);

		//// Serialize the children recursively
		//std::vector<Entity> children = entity.GetChildren();
		//archive((int)children.size());

		//for (Entity child : children)
		//{
		//	SerializeEntityHierarchy(child, archive);
		//}
	}

}
