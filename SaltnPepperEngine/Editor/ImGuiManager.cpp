#include "ImGuiManager.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Utils/Logging/Log.h"
#include <imgui/imgui.h>


namespace SaltnPepperEngine
{
	namespace Editor
	{
		static const char* ImGui_ImplGlfw_GetClipboardText(void*)
		{
			return glfwGetClipboardString(Application::GetCurrent().GetAppWindow().GetHandle());
		}

		static void ImGui_ImplGlfw_SetClipboardText(void*, const char* text)
		{
			glfwSetClipboardString(Application::GetCurrent().GetAppWindow().GetHandle(), text);
		}

		int GetImGuiMouseBind(Input::MouseButton button)
		{
			switch (button)
			{
			case Input::MouseButton::Left: return 0;

				break;
			case Input::MouseButton::Middle: return 1;
				break;
			case Input::MouseButton::Right: return 2;
				break;
			case Input::MouseButton::NoCursor:
				break;
			default: return 4;

				break;
			}

			return 4;
		}


		ImGuiManager::ImGuiManager(bool clearScreen)
		{
			m_clearScreen = clearScreen;
			m_fontSize = 15.0f;

		}

		ImGuiManager::~ImGuiManager()
		{

		}


		void ImGuiManager::Init()
		{
			LOG_INFO("ImGui Loaded with version : {0}", IMGUI_VERSION);
		
			Application& app = Application::GetCurrent();
			ImGuiIO& io = ImGui::GetIO();

			uint32_t width = app.GetAppWindow().GetSize().x;
			uint32_t height = app.GetAppWindow().GetSize().y;


			io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			SetImGuiKeyBinds();
			SetImGuiStyle();

			m_imguiRenderer = MakeUnique<ImGuiRenderer>(width, height,m_clearScreen);

			if (m_imguiRenderer)
			{	
				m_imguiRenderer->SetWindowRef(Application::GetCurrent().GetAppWindow().GetHandle());
				m_imguiRenderer->Init();

			}

			io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
			io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;

		}

		void ImGuiManager::Update(const float deltaTime, Scene* scene)
		{
			if (m_imguiRenderer)
			{
				m_imguiRenderer->NewFrame();
			}

			Application::GetCurrent().OnImGui();
		}


		void ImGuiManager::OnEvent(EventBase& event)
		{
			EventDispatcher dispatcher(event);
			dispatcher.dispatch<MouseButtonDownEvent>(BIND_FN(ImGuiManager::OnMouseClickEvent));
			dispatcher.dispatch<MouseButtonUpEvent>(BIND_FN(ImGuiManager::OnMouseReleasedEvent));
			dispatcher.dispatch<MouseMovedEvent>(BIND_FN(ImGuiManager::OnMouseMovedEvent));
			dispatcher.dispatch<MouseScrolledEvent>(BIND_FN(ImGuiManager::OnMouseScrolledEvent));
			dispatcher.dispatch<KeyDownEvent>(BIND_FN(ImGuiManager::OnKeyPressedEvent));
			dispatcher.dispatch<KeyUpEvent>(BIND_FN(ImGuiManager::OnKeyReleasedEvent));
			dispatcher.dispatch<CharTypedEvent>(BIND_FN(ImGuiManager::OnKeyTypedEvent));
			dispatcher.dispatch<WindowResizeEvent>(BIND_FN(ImGuiManager::OnWindowResizeEvent));
		}


		void ImGuiManager::OnRender(Scene* scene)
		{
			if (m_imguiRenderer)
			{
				m_imguiRenderer->Render();
			}
		}


	
		bool ImGuiManager::OnMouseClickEvent(MouseButtonDownEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[GetImGuiMouseBind(event.GetMouseButton())] = true;
			return false;
		}
		bool ImGuiManager::OnMouseReleasedEvent(MouseButtonUpEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[GetImGuiMouseBind(event.GetMouseButton())] = false;
			return false;
		}
		bool ImGuiManager::OnMouseMovedEvent(MouseMovedEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			
			if (Input::InputSystem::GetInstance().GetMouseState() == MouseState::Visible)
			{
				io.MousePos = ImVec2(event.GetXAxis(), event.GetYAxis());
			}

			return false;
		}
		bool ImGuiManager::OnMouseScrolledEvent(MouseScrolledEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();

			io.MouseWheel += event.GetYDelta() / 10.0f;
			io.MouseWheelH += event.GetXDelta() / 10.0f;
			return false;
		}
		bool ImGuiManager::OnKeyPressedEvent(KeyDownEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();

			io.KeysDown[(int)event.GetKeyCode()] = true;

			io.KeyCtrl = io.KeysDown[(int)Input::Key::LeftControl] || io.KeysDown[(int)Input::Key::RightControl];
			io.KeyShift = io.KeysDown[(int)Input::Key::LeftShift] || io.KeysDown[(int)Input::Key::RightShift];
			io.KeyAlt = io.KeysDown[(int)Input::Key::LeftAlt] || io.KeysDown[(int)Input::Key::RightAlt];
			
			
			return io.WantTextInput;
		}

		bool ImGuiManager::OnKeyReleasedEvent(KeyUpEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[(int)event.GetKeyCode()] = false;

			return false;
		}

		bool ImGuiManager::OnKeyTypedEvent(CharTypedEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			int keyCode = (int)event.character;

			if (keyCode > 0 && keyCode < 0x100000)
			{
				io.AddInputCharacter((unsigned short)keyCode);
			}


			return false;
		}
		bool ImGuiManager::OnWindowResizeEvent(WindowResizeEvent& event)
		{
			ImGuiIO& io = ImGui::GetIO();
			return false;
		}
		void ImGuiManager::SetImGuiKeyBinds()
		{
		}


		void ImGuiManager::SetImGuiStyle()
		{
		}


	}
}