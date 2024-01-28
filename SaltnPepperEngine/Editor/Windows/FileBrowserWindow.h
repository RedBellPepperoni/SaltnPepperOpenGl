#ifndef FILEBROWSERWINDOW_H
#define FILEBROWSERWINDOW_H

#include "Editor/EditorWindow.h"
#include <functional>
#include "Engine/Utils/Loading/FileSystem.h"

namespace ImGui
{
	class FileBrowser;
}


namespace SaltnPepperEngine
{
	namespace Editor
	{
		class FileBrowserWindow : public EditorWindow
		{

		public:

			FileBrowserWindow();
			~FileBrowserWindow();


			void Open();
			void OnImgui() override;

			void SetCurrentPath(const std::string& path);
			void OpenDirectory(bool value);

			void SetCallback(const std::function<void(const std::string&)>& callback);

			bool IsOpen();
			void SetFileTypeFilters(const std::vector<std::string>& filters);
			void ClearFileFilters();
			
			const FilePath& GetPath();

		private:

			std::function<void(const std::string&)> m_callback;
			ImGui::FileBrowser* m_fileBrowser;
		};
	}
}

#endif // !FILEBROWSERWINDOW_H



