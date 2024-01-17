#include "HierarchyWindow.h"
#include "Engine/Core/EntitySystem/Entity.h"

namespace SaltnPepperEngine
{

	namespace Editor
	{
		HierarchyWindow::HierarchyWindow()
			: m_hadDroppedEntity(entt::null)
			, m_doubleClicked(entt::null)

		{
			m_name = "Hierarchy";
		}



		void HierarchyWindow::DrawEntity(entt::entity entity, entt::registry& registry)
		{
		}

		void HierarchyWindow::OnImgui()
		{
			static bool start = true;
			ImGui::Begin("Hierarchy",&start);

			ImGui::Text("Hellllllloooooooo !!!!");

			ImGui::End();

		}
	
		bool HierarchyWindow::IsParentOfEntity(entt::entity entity, entt::entity child, entt::registry& registry)
		{
			return false;
		}
	}
}

