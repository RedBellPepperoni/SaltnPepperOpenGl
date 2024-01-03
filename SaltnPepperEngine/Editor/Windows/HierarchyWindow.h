#ifndef HIERARCHYWINDOW_H
#define HIERARCHYWINDOW_H
#include "Editor/EditorWindow.h"
#include <entt/entity/fwd.hpp>
#include <Imgui/imgui.h>


namespace SaltnPepperEngine
{

	namespace Editor
	{

		class HierarchyWindow : public EditorWindow
		{
		private:

			ImGuiTextFilter m_hierarchyFilter;

			entt::entity m_doubleClicked;
			entt::entity m_hadDroppedEntity;
			entt::entity m_currentPrevious;

			bool m_selectUp;
			bool m_selectDown;


		public:

			HierarchyWindow();
			virtual ~HierarchyWindow() = default;

			void DrawEntity(entt::entity entity, entt::registry& registry);
			void OnImgui() override;
			bool IsParentOfEntity(entt::entity entity, entt::entity child, entt::registry& registry);



		};

	}
}


#endif // !HIERARCHYWINDOW_H
