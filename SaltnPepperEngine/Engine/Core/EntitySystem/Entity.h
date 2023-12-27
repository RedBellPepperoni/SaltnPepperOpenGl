#ifndef ENTITY_H
#define ENTITY_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Utils/Maths/MathDefinitions.h"


#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Scene/SceneGraph.h"

namespace SaltnPepperEngine
{
	using namespace Components;




	// Header Only Entity class wrapping arounf the entt system
	class Entity
	{
	private:

		friend class EntityManager;

		entt::entity m_entityHandle = entt::null;
		Scene* m_scene;





	public:

		Entity() = default;

		Entity(entt::entity handle, Scene* scene)
			:m_entityHandle(handle)
			, m_scene(scene)
		{

		}

		~Entity()
		{

		}

		bool IsActive()
		{
			bool active = true;

			if (HasComponent<ActiveComponent>())
			{
				active = GetComponent<ActiveComponent>().active;
;			}

			Entity parent = GetParent();

			// Recursively Check if the PArent is active or not
			if (parent)
			{
				active &= parent.IsActive();
			}

			return active;
		}

		void SetActive(const bool newactive)
		{
			ActiveComponent* comp = TryGetComponent<ActiveComponent>();
			
			if (comp)
			{
				comp->active = newactive;
			}

		}


		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			if (HasComponent<T>())
			{
				LOG_WARN("Trying to add the component more than once");
			}

			return m_scene->GetRegistry().emplace <T>(m_entityHandle, std::forward<Args>(args)...);

		}

		template<typename T>
		T& GetComponent()
		{
			// Return component here
			return m_scene->GetRegistry().get<T>(m_entityHandle);
		}

		template<typename T>
		T* TryGetComponent()
		{
			return m_scene->GetRegistry().try_get<T>(m_entityHandle);
		}


		template <typename T>
		bool HasComponent()
		{

			return m_scene->GetRegistry().all_of<T>(m_entityHandle);
		}


		template<typename T>
		void RemoveComponent()
		{
			m_scene->GetRegistry().remove<T>(m_entityHandle);
		}

		template<typename T>
		void TryRemoveComponent()
		{
			if (HasComponent<T>())
			{
				RemoveComponent<T>();
			}
		}


		Transform& GetTransform()
		{
			return m_scene->GetRegistry().get<Transform>(m_entityHandle);
		}

		const Transform& GetTransfomr() const
		{
			return m_scene->GetRegistry().get<Transform>(m_entityHandle);
		}

		uint64_t GetId()
		{
			return m_scene->GetRegistry().get<IdComponent>(m_entityHandle).ID.GetId();
		}

		const std::string& GetName()
		{

			NameComponent* comp = TryGetComponent<NameComponent>();

			if (comp)
			{
				return comp->name;
			}
			else
			{
				return "unamedEntity";
			}

		}

		void SetParent(Entity entity)
		{
			// Add parents here

			bool acceptable = false;
			Hierarchy* hierarchyComponent = TryGetComponent<Hierarchy>();
			if (hierarchyComponent != nullptr)
			{
				acceptable = entity.m_entityHandle != m_entityHandle && (!entity.IsParent(*this)) && (hierarchyComponent->Parent() != m_entityHandle);
			}
			else
				acceptable = entity.m_entityHandle != m_entityHandle;

			if (!acceptable)
			{
				LOG_WARN("Failed to parent entity!");
				return;
			}

			if (hierarchyComponent)
				Hierarchy::Reparent(m_entityHandle, entity.m_entityHandle, m_scene->GetRegistry(), *hierarchyComponent);
			else
			{
				m_scene->GetRegistry().emplace<Hierarchy>(m_entityHandle, entity.m_entityHandle);
			}

		}

		Entity GetParent()
		{
			Hierarchy* hierarchyComp = TryGetComponent<Hierarchy>();
			if (hierarchyComp)
				return Entity(hierarchyComp->Parent(), m_scene);
			else
				return Entity(entt::null, nullptr);
		}

		std::vector<Entity> GetChildren()
		{
			std::vector<Entity> children;
			Hierarchy* hierarchyComponent = TryGetComponent<Hierarchy>();
			if (hierarchyComponent)
			{
				entt::entity child = hierarchyComponent->First();
				while (child != entt::null && m_scene->GetRegistry().valid(child))
				{
					children.emplace_back(child, m_scene);
					hierarchyComponent = m_scene->GetRegistry().try_get<Hierarchy>(child);
					if (hierarchyComponent)
						child = hierarchyComponent->Next();
				}
			}

			return children;
		}

		void ClearChildren()
		{
			Hierarchy* hierarchyComponent = TryGetComponent<Hierarchy>();
			if (hierarchyComponent)
			{
				hierarchyComponent->m_First = entt::null;
			}
		}

		bool IsParent(Entity parentcheck)
		{
			Hierarchy* nodeHierarchyComponent = m_scene->GetRegistry().try_get<Hierarchy>(m_entityHandle);
			
			if (nodeHierarchyComponent)
			{
				auto parent = nodeHierarchyComponent->Parent();
				while (parent != entt::null)
				{
					if (parent == parentcheck.m_entityHandle)
					{
						return true;
					}
					else
					{
						nodeHierarchyComponent = m_scene->GetRegistry().try_get<Hierarchy>(parent);
						parent = nodeHierarchyComponent ? nodeHierarchyComponent->Parent() : entt::null;
					}
				}
			}

			return false;
		}

		entt::entity GetHandle()
		{
			return m_entityHandle;
		}

		void Destroy()
		{
			m_scene->GetRegistry().destroy(m_entityHandle);
		}

		bool IsValid()
		{
			return m_scene->GetRegistry().valid(m_entityHandle) && m_scene;
		}

		Scene* GetScene() const
		{
			return m_scene;
		}



		operator entt::entity() const
		{
			return m_entityHandle;
		}

		operator bool() const
		{
			return((m_entityHandle != entt::null) && m_scene);
		}

		bool operator==(const Entity& other) const
		{
			return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	};
}

#endif // !ENTITY_H
