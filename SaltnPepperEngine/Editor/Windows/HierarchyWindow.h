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

			// Text Filter for Hierarchy Elements
			ImGuiTextFilter m_hierarchyFilter;

			// Entity , if its double clicked in the window
			entt::entity m_doubleClicked;

			// If an Entity has been recently Drag-Dropped
			entt::entity m_hadDroppedEntity;

			// The Reference to the Previous
			entt::entity m_currentPrevious;

			// Key Bind if the next entity in list is selected
			bool m_selectUp;

			// Keybind to check if previous  entity in the list is selected
			bool m_selectDown;


		public:

			HierarchyWindow();
			virtual ~HierarchyWindow() = default;

			// Draws the Dynamic node for Each entity
			void DrawNode(entt::entity entity, entt::registry& registry);

			// Main Draw Function call for Imgui
			void OnImgui() override;

			// Check to see if a entity is a parent of another entity
			bool IsParentOfEntity(entt::entity entity, entt::entity child, entt::registry& registry);



		};

	}
}


#endif // !HIERARCHYWINDOW_H
