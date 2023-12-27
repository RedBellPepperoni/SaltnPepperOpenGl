#include "EntityManager.h"

namespace SaltnPepperEngine
{
	// Create Without a specifc name, just an id
	Entity EntityManager::Create()
	{

		// Create a new Entity
		entt::entity newEntity = m_registry.create();

		//Add a ID Component to give an unique ID to the newly created Entity
		m_registry.emplace<IdComponent>(newEntity);


		return Entity(newEntity, m_scene);
	}

	// create a new entity with a specific name and ID
	Entity EntityManager::Create(const std::string& name)
	{
		// Create a new Entity
		entt::entity newEntity = m_registry.create();

		//Add a Name Component t oname the entity using the provided name
		m_registry.emplace<NameComponent>(newEntity, name);

		//Add a ID Component to give an unique ID to the newly created Entity
		m_registry.emplace<IdComponent>(newEntity);

		return Entity(newEntity, m_scene);
	}


	void EntityManager::Clear()
	{
		// Go thriugh all the entities in the Registry
		for (auto [entity] : m_registry.storage<entt::entity>().each())
		{
			// Destroy each entity
			m_registry.destroy(entity);
		}

		// Finally clear the Registry of any dangling reference
		m_registry.clear();
	}

	// Getter for the Registry
	entt::registry& EntityManager::GetRegistry()
	{
		return m_registry;
	}

	// Find a particular entity using its unique identifier
	Entity EntityManager::GetEntitybyId(uint64_t id)
	{
		// using auto here since the type chain is too long
		auto view = m_registry.view<IdComponent>();

		// We loop through all the entities in the current registry
		for (entt::entity entity : view)
		{
			// similarly the type chain is too long, so using auto instead
			// Should give us a referernce to the stored Id componets on the entity
			auto& idComponent = m_registry.get<IdComponent>(entity);

			// If the Id matches .. return the found entity
			if (idComponent.ID.GetId() == id)
				return Entity(entity, m_scene);
		}

		// No id match was found, so we just out put a warning and send a new Entity

		LOG_WARN("No entity with the Given Id was found");

		return Entity();
	}

	Entity EntityManager::GetEntitybyHandle(const uint32_t& handle)
	{
		entt::entity derefEntity = (entt::entity)handle;

		if (m_registry.valid(derefEntity))
		{
			return Entity(derefEntity, m_scene);
		}

		LOG_WARN("No entity with the Given handle was found");
		return Entity();
	}






}