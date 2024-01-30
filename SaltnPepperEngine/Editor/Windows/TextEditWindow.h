#ifndef TEXTEDITWINDOW_H
#define TEXTEDITWINDOW_H
#include "Editor/EditorWindow.h"

namespace SaltnPepperEngine
{
	namespace Editor
	{
		class TextEditorWindow : public EditorWindow
		{
			TextEditorWindow();
			virtual ~TextEditorWindow() = default;

			virtual void OnImgui() override;

			void OnClose();

			/*void OnSave(const std::is_function<void()>& callback);
			void SetErrors(const std::map<>)
*/

		};

	}
}

#endif // TEXTEDITWINDOW_H
