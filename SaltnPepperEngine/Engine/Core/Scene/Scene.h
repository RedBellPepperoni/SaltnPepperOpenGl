#ifndef SCENE_H
#define SCENE_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Macros.h"
#include "Engine/Utils/Serialization/Serialization.h"
#include "cereal/archives/json.hpp"

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

	namespace Rendering 

	{	
		class Camera;
		class DirectionalLight;
	}
	class SceneGraph;

	namespace Components
	{
		class Transform;
	}

	/*namespace Audio
	{
		class AudioListener;
	}*/

	using Components::Transform;
	using Rendering::DirectionalLight;
	using Rendering::Camera;
	//using Audio::AudioListener;


	class Scene
	{

	private:

		// Name of the Scene
		std::string m_name;
		int m_sceneSerializationVersion = 0;
		// The Refernce to the Entity Manager wrapper class 
		UniquePtr<EntityManager> m_EntityManager;

		UniquePtr<SceneGraph> m_SceneGraph;

		//SharedPtr<DirectionalLight> m_directionLight = nullptr;


		Transform* mainCameraTransform = nullptr;
		CameraController* mainCameraController = nullptr;
		//AudioListener* mainAudioListener = nullptr;

		void DestroyInternal(entt::entity, entt::registry& registry);



	public:

		

		// Should make this explicit later on to avoid unnamed scenes
		explicit Scene(const std::string& name);

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

		void SetMainCamera(CameraController* controller, Transform* transform);
		void SetMainCameraPosition(const Vector3 position);

		Camera* GetMainCamera();
		Transform* GetMainCameraTransform();
		


		void UpdateSceneGraph();



		const int GetSceneVersion() const
		{
			return m_sceneSerializationVersion;
		}

		void Serialize(const std::string& filename, bool binary = false);
		void Deserialize(const std::string filename,bool binary = false);

		


		template<typename Archive>
		void save(Archive& archive) const
		{
			/*archive(cereal::make_nvp("Version", SceneSerializarionVersion));
			archive(cereal::make_nvp("Scene Name", m_name));*/
		}

		template<typename Archive>
		void load(Archive& archive) const
		{
			/*archive(cereal::make_nvp("Version", m_sceneSerializationVersion));
			archive(cereal::make_nvp("Scene Name", m_name));

			CurrentSceneVersion = m_sceneSerializationVersion;*/
		}


	};

}



#endif // !SCENE_H



