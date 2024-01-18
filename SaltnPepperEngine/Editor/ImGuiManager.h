#ifndef IMGUIMANAGER_H
#define IMGUIMANAGER_H
#include "Engine/Core/System/Events/KeyboardEvent.h"
#include "Engine/Core/System/Events/MouseEvent.h"
#include "Engine/Core/System/Events/WindowEvent.h"
#include "Engine/Core/System/Events/Event.h"
#include "ImGuiRenderer.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{
	class Scene;
	

	namespace Editor
	{
		class ImGuiManager
		{
		public:
			ImGuiManager(bool clearScreen = false);
			~ImGuiManager();

			void Init();
			void Update(const float deltaTime, Scene* scene);
			void OnEvent(EventBase& event);
			void OnRender(Scene* scene);


		private:

			bool OnMouseClickEvent(MouseButtonDownEvent& event);
			bool OnMouseReleasedEvent(MouseButtonUpEvent& event);
			bool OnMouseMovedEvent(MouseMovedEvent& event);
			bool OnMouseScrolledEvent(MouseScrolledEvent& event);

			bool OnKeyPressedEvent(KeyDownEvent& event);
			bool OnKeyReleasedEvent(KeyUpEvent& event);
			bool OnKeyTypedEvent(CharTypedEvent& event);

			bool OnWindowResizeEvent(WindowResizeEvent& event);


			void SetImGuiKeyBinds();
			void SetImGuiStyle();
			void AddIconFont();
			float m_fontSize = 1.0f;
			float m_DPIScale = 1.0f;

			

			UniquePtr<ImGuiRenderer> m_imguiRenderer;
			bool m_clearScreen;

		};
	}
}

#endif // !IMGUIMANAGER_H

