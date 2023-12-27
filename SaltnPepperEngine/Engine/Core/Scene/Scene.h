#ifndef SCENE_H
#define SCENE_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Macros.h"


// GEt the Entt Registry header
DISABLE_PUSH_WARNING
DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE
#include <entt/entity/registry.hpp>
DISABLE_POP_WARNING

namespace SaltnPepperEngine
{
	class EntityManager;
	class Entity;
	class CameraController;
	class DirectionLight;
	class SceneGraph;

	namespace Components
	{
		class Transform;
	}

	namespace Audio
	{
		class AudioListener;
	}

	using Components::Transform;
	using Audio::AudioListener;


	class Scene
	{

	private:

		// Name of the Scene
		std::string m_name;

		// The Refernce to the Entity Manager wrapper class 
		UniquePtr<EntityManager> m_EntityManager;

		UniquePtr<SceneGraph> m_SceneGraph;

		SharedPtr<DirectionLight> m_directionLight = nullptr;




		Transform* mainCameraTransform = nullptr;
		CameraController* mainCameraController = nullptr;
		AudioListener* mainAudioListener = nullptr;

		void DestroyInternal(entt::entity, entt::registry& registry);



	public:

		// Should make this explicit later on to avoid unnamed scenes
		Scene(const std::string& name);

		// Destructor
		~Scene();

		// The initialzation of the scen and registries
		void Init();

		// the deletion of the scene and registries
		void CleanUp();

		// this should be called everyframe to update teh trasnforms and such
		void Update(float deltaTime);

		// getter for accessing the scene name
		const std::string& GetName() const;

		// Setter to change the scene name
		void SetName(const std::string& name);

		// Wrapper around the entt registry
		entt::registry& GetRegistry();

		// Creates and returns a new entity
		Entity CreateEntity();

		// Create a new entity with the provided name and return it
		Entity CreateEntity(const std::string& name);

		// Duplicates a given Entity
		void Duplicate(Entity entity);

		// Duplicates with Parent Chain
		void Duplicate(Entity entity, Entity parent);

		// Deletes/ Destoys given entity 
		void DestroyEntity(Entity entity);


		// Find a entity by the given ID if it exists
		Entity GetEntitybyId(const uint64_t id);

		// The publically accessible Pointer to the Entity manager
		EntityManager* GetEntityManager();

		// Loops through all the Entitys in the scene and destroys them
		void DestroyAllGameObjects();

		void SetMainCamera(CameraController* controller, Transform* transform, AudioListener* listener);

		void SetMainCameraPosition(const Vector3 position);

		void UpdateSceneGraph();



		void Serialize(const std::string& filename);
		void Deserialize(const std::string filename);

		Transform* GetMainCameraTransform() const;

	};

}



#endif // !SCENE_H



