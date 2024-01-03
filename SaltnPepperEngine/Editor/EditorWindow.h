#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H
#include <string>

namespace SaltnPepperEngine
{
	class Scene;

	namespace Editor
	{
		class Editor;



		class EditorWindow
		{

		protected:

			bool m_active = true;
			std::string m_name = "DefaultWindow";


		public:

			virtual ~EditorWindow() = default;

			virtual void OnImgui() = 0;


			virtual void OnInit();
			virtual void OnUpdate();

			const bool GetActive() const;
			void SetActive(bool active);

			const std::string& GetName() const;


		};

	}
}


#endif // !EDITORWINDOW_H
