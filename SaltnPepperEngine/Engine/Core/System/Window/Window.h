#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <string>
#include "Engine/Core/System/Events/Event.h"
#include "Engine/Utils/Maths/MathDefinitions.h"

// Forward Declaring stuff for faster compilation
struct GLFWwindow;

namespace SaltnPepperEngine
{

	namespace Rendering
	{



		class Window
		{
		public:

			using EventCallBackFn = std::function<void(EventBase&)>;

			struct WindowProperties
			{

				int width = 1920;
				int height = 1200;
				bool hasVSync = false;
				bool isFullscreen = false;
				std::string title = "Salt n Pepper Engine | Open GL";
				float aspectRatio;
				EventCallBackFn eventHandler;
			};


			bool wireFrame = false;

		protected:



			GLFWwindow* windowHandle;


			WindowProperties m_properties;
			bool m_windowFoucs = true;


			// ============================ EVENT WRAPPERS  ==========================

			static inline void WindowSizeCallback(GLFWwindow* window, int newWidth, int newHeight);

			static inline void WindowCloseCallback(GLFWwindow* window);

			static inline void WindowFocusCallback(GLFWwindow* window, int focused);

			static inline void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			static inline void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

			static inline void WindowMouseScrollCallback(GLFWwindow* window, double xDelta, double yDelta);

			static inline void WindowCursorCallback(GLFWwindow* window, double xPos, double yPos);


		public:



			Window();
			Window(const WindowProperties& properties);
			~Window();

			void Initialize();

			void UpdateViewPort();
			void PollEvents();

			void SetEventCallback(const EventCallBackFn& callback);
			void CallEvent(EventBase& event);


			void InitializeImGui();
			void UpdateImGui();
			void RenderImGui();

			bool isOpen();
			void SwapBuffers();
			void CloseWindow();

			GLFWwindow* GetHandle() { return windowHandle; }

			float GetAspectRatio() { return m_properties.aspectRatio; };

			float GetGLFWTime() const;

			void SetWindowTitle(const std::string& newTitle);
			void SetMousePosition(Vector2 position);
			void SetMouseHidden(bool isHidden);
			void ToggleWireframe();

			const Vector2Int GetSize() const;


			const bool GetVsync() const { return m_properties.hasVSync; }
			const bool GetFullScreen() const { return m_properties.isFullscreen; }
			const bool GetFocus() const { return m_windowFoucs; }
			void SetFocus(const bool focus) { m_windowFoucs = focus; }

			void SetVsync(bool isVsync);

		};

	}
}

#endif