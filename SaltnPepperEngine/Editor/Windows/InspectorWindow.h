#ifndef INSPECTORWINDOW_H
#define INSPECTORWINDOW_H
#include "Editor/EditorWindow.h"
#include "Editor/Windows/EntityEditorUtils.hpp"

#include <entt/entt.hpp>

namespace SaltnPepperEngine
{

	namespace Editor
	{
		

		class InspectorWindow : public EditorWindow
		{
		public :

			InspectorWindow();
			virtual ~InspectorWindow() = default;

			virtual void OnImgui() override;


			virtual void OnInit();
			virtual void OnUpdate();

			void SetDebugMode(bool mode);
			bool GetIsDebug() const;

		private:

			MM::EntityEditor<entt::entity> m_EntityEditor;
			bool m_debugMode = false;
		
		};
	}
}


#endif // !INSPECTORWINDOW_H
