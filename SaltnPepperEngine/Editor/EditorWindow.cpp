#include "EditorWindow.h"


namespace SaltnPepperEngine
{

	namespace Editor
	{
		void EditorWindow::OnInit()
		{
		}

		void EditorWindow::OnUpdate()
		{

		}

		const bool EditorWindow::GetActive() const
		{
			return m_active;
		}
		void EditorWindow::SetActive(bool active)
		{
			m_active = active;
		}

		const std::string& EditorWindow::GetName() const
		{
			return m_name;
		}
	}
}
