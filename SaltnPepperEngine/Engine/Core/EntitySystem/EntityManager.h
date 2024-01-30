#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include <string>

//// GEt the Entt Registry header
DISABLE_PUSH_WARNING
DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE
#include <entt/entity/registry.hpp>
DISABLE_POP_WARNING


namespace SaltnPepperEngine
{
	class Scene;
	class Entity;

	template <typename... Component>
	struct ComponentView
	{
		class iterator;
		using View = entt::view<entt::get_t<Component...>>;

	public:

		Scene* m_scene = nullptr;
		View m_view;

		ComponentView(Scene* scene)
			: m_scene(scene)
			, m_view(scene->GetRegistry().view<Component...>())
		{

		}

		Entity operator[](int i)
		{
			// Out of bounds : This should realistically not happen
			if (i >= Size())
			{
				LOG_ERROR("Index out of bound on EntityView");
				__debugbreak();
			}

			return Entity(m_view[i], m_scene);
		}

		bool IsEmpty() const
		{
			return m_view.empty();
		}

		size_t Size() const
		{
			return m_view.size();
		}

		iterator begin()
		{
			return ComponentView<Component...>::iterator(*this, 0);
		}

		iterator end()
		{
			return ComponentView<Component...>::iterator(*this, Size());
		}

		// Custom class for iterating through our
		class iterator
		{
		public:

			using pointer = Entity*;
			using reference = Entity&;

			explicit iterator(ComponentView<Component...>& view, size_t index = 0)
				: m_View(view)
				, m_Index(index)
			{
			}

			Entity operator*() const
			{
				return m_View[int(m_Index)];
			}

			iterator& operator++()
			{
				m_Index++;
				return *this;
			}

			iterator operator++(int)
			{
				iterator temp = *this;
				++(*this);
				return temp;
			}

			bool operator!=(const iterator& rhs) const
			{
				return m_Index != rhs.m_Index;
			}
		private:
			size_t m_Index = 0;
			ComponentView<Component...>& m_View;
		};


	};





	class EntityManager
	{
	private:

		Scene* m_scene = nullptr;
		entt::registry m_registry;

	public:

		EntityManager(Scene* scene)
			: m_scene(scene)
		{
			m_registry = {};
		}

		Entity Create();
		Entity Create(const std::string& name);


		void Clear();

		template <typename... Components>
		auto GetComponentsOfTypes()
		{
			return m_registry.group<Components...>();
		}


		template<typename... Component>
		ComponentView<Component...> GetComponentsOfType()
		{
			return ComponentView<Component...>(m_scene);
		}

		template <typename R, typename T>
		void AddComponentOnConstructDependency()
		{
			m_registry.template on_construct<R>().template connect<&entt::registry::get_or_emplace<T>>();
		}

		// Getter for the Registry
		entt::registry& GetRegistry()
		{
			return m_registry;
		}

		Entity GetEntitybyId(uint64_t id);

		Entity GetEntitybyHandle(const uint32_t& handle);

		template<typename Component>
		Component* TryGet(entt::entity entity)
		{
			return m_registry.try_get<Component>(entity);
		}

	};


}


#endif // !ENTITYMANAGER_H