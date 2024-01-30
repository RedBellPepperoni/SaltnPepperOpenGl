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

		void EditorWindow::Release()
		{
		}

		bool& EditorWindow::GetActive() 
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

		void EditorWindow::SetEditor(RuntimeEditor* editor)
		{
			m_editorHandle = editor;
		}
		RuntimeEditor* EditorWindow::GetEditor()
		{
			return m_editorHandle;
		}
	}
}
