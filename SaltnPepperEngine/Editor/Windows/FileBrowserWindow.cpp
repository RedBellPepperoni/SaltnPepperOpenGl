#include "FileBrowserWindow.h"
#include "Editor/MDIcons.h"
#include <Imgui/imgui.h>
#include <Imgui/ImFileBrowser.h>


namespace SaltnPepperEngine
{
	namespace Editor
	{
		FileBrowserWindow::FileBrowserWindow()
		{
			m_name = "FileBrowserWindow";
			m_fileBrowser = new ImGui::FileBrowser(ImGuiFileBrowserFlags_CreateNewDir | ImGuiFileBrowserFlags_EnterNewFilename);
			m_fileBrowser->SetTitle("File Browser");
			//m_fileBrowser->label(ICON_MDI_FOLDER, ICON_MDI_FILE, ICON_MDI_FOLDER_OPEN);


			// Refresh the file system
			//m_fileBrowser->SetPwd();
		
		}
		FileBrowserWindow::~FileBrowserWindow()
		{
			delete m_fileBrowser;
		}

		void FileBrowserWindow::Open()
		{
			m_fileBrowser->Open();
		}

		void FileBrowserWindow::OnImgui()
		{
			m_fileBrowser->Display();

			if (m_fileBrowser->HasSelected())
			{
				std::string tempFilePath = m_fileBrowser->GetSelected().string();

				std::string filePath = (tempFilePath);

				m_callback(filePath);

				m_fileBrowser->ClearSelected();
			}
		}

		void FileBrowserWindow::SetCurrentPath(const std::string& path)
		{
			m_fileBrowser->SetPwd(path);
		}

		void FileBrowserWindow::OpenDirectory(bool value)
		{
			//m_fileBrowser->
		}

		void FileBrowserWindow::SetCallback(const std::function<void(const std::string&)>& callback)
		{
			m_callback = callback;
		}

		bool FileBrowserWindow::IsOpen()
		{
			return m_fileBrowser->IsOpened();
		}

		void FileBrowserWindow::SetFileTypeFilters(const std::vector<std::string>& filters)
		{
			m_fileBrowser->SetTypeFilters(filters);
		}

		void FileBrowserWindow::ClearFileFilters()
		{
			//m_fileBrowser->ClearFilters();
		}

		const FilePath& FileBrowserWindow::GetPath()
		{
			return m_fileBrowser->GetPwd();
		}
	}
}