#include "Scene.h"
#include "SceneGraph.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/System/Input/InputSystem.h"

// All the Component Includes
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Camera/FlyCameraController.h"

#include "Engine/Core/System/Application/Application.h"


namespace SaltnPepperEngine
{
	
	using namespace Rendering;
	using namespace Components;

	Scene::Scene(const std::string& name)
	{
		// Setting up a new Entity Manager with the current scene's Reference
		m_EntityManager = MakeUnique<EntityManager>(this);
		

		m_SceneGraph = MakeUnique<SceneGraph>();
		m_SceneGraph->Init(GetRegistry());


		//m_EntityManager->AddComponentOnConstructDependency<Transform>()


	}

	Scene::~Scene()
	{
		// Cleanup the manager
		m_EntityManager->Clear();
	}

	void Scene::Init()
	{
		Entity cameraEntity = GetEntityManager()->Create("MainCamera");
		Camera* camera = &cameraEntity.AddComponent<Camera>();
		Transform* transform = &cameraEntity.AddComponent<Transform>();

		

		////AudioListener* listener = &cameraEntity.AddComponent<Audio::AudioListener>(transform);


		transform->SetPosition(Vector3(2.2f, 0.97f, 0.65f));
		transform->SetEularRotation(Vector3(-45.0f, 3.6f, 0.0f));

		CameraController& controller = cameraEntity.AddComponent<FlyCameraController>();
		controller.SetCamera(camera);

		//cameraEntity.AddComponent<DefaultCameraController>(DefaultCameraController::CameraType::Orbital);


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
		//ComponentView audioListenerView = m_EntityManager->GetComponentsOfType<AudioListener>();


		Camera* camera = nullptr;

		if (!cameraview.IsEmpty())
		{
			camera = &cameraview[0].GetComponent<Camera>();
		}




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
	}

	void Scene::Duplicate(Entity entity, Entity parent)
	{
		
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






	void Scene::Serialize(const std::string& filename)
	{
	}
	void Scene::Deserialize(const std::string filename)
	{
	}

	Transform* Scene::GetMainCameraTransform() const
	{
		return mainCameraTransform;
	}
}