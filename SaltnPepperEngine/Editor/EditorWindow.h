#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H
#include <string>

namespace SaltnPepperEngine
{
	class Scene;

	namespace Editor
	{
		class RuntimeEditor;

		class EditorWindow
		{

		protected:

			bool m_active = true;
			std::string m_name = "DefaultWindow";
			RuntimeEditor* m_editorHandle = nullptr;



		public:

			virtual ~EditorWindow() = default;

			virtual void OnImgui() = 0;

			virtual void OnInit();
			virtual void OnUpdate();

			virtual void Release();

			bool& GetActive() ;
			void SetActive(bool active);

			const std::string& GetName() const;

			void SetEditor(RuntimeEditor* editor);
			RuntimeEditor* GetEditor();


		
		};

	}
}


#endif // !EDITORWINDOW_H
