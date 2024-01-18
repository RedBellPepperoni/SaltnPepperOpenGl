#include "ImGuiManager.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/System/Window/Window.h"
#include "Engine/Utils/Logging/Log.h"
#include <Imgui/imgui.h>
#include "MDIcons.h"
#include "ImGuiUtils.h"

#include <Imgui/Plugins/ImGuiAl/fonts/MaterialDesign.inl>
#include <Imgui/Plugins/ImGuiAl/fonts/RobotoMedium.inl>
#include <Imgui/Plugins/ImGuiAl/fonts/RobotoRegular.inl>
#include <Imgui/Plugins/ImGuiAl/fonts/RobotoBold.inl>



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
		
			ImGuiContext* context = ImGui::CreateContext();
			ImGui::SetCurrentContext(context);

		

			Application& app = Application::GetCurrent();
			ImGuiIO& io = ImGui::GetIO();

			uint32_t width = app.GetAppWindow().GetSize().x;
			uint32_t height = app.GetAppWindow().GetSize().y;

			m_imguiRenderer = MakeUnique<ImGuiRenderer>(m_clearScreen);

			if (m_imguiRenderer)
			{		
				m_imguiRenderer->Init(Application::GetCurrent().GetAppWindow().GetHandle());
			}
			io.DisplaySize = ImVec2(static_cast<float>(width), static_cast<float>(height));
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.ConfigWindowsMoveFromTitleBarOnly = true;

			SetImGuiKeyBinds();
			SetImGuiStyle();

		

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
			ImGuiIO& io = ImGui::GetIO();

			ImGui::StyleColorsDark();

			io.FontGlobalScale = 1.0f;

			ImFontConfig icons_config;
			icons_config.MergeMode = false;
			icons_config.PixelSnapH = true;
			icons_config.OversampleH = icons_config.OversampleV = 1;
			icons_config.GlyphMinAdvanceX = 4.0f;
			icons_config.SizePixels = 12.0f;

			static const ImWchar ranges[] = {
				0x0020,
				0x00FF,
				0x0400,
				0x044F,
				0,
			};

			io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, m_fontSize, &icons_config, ranges);
			AddIconFont();

			io.Fonts->AddFontFromMemoryCompressedTTF(RobotoBold_compressed_data, RobotoBold_compressed_size, m_fontSize + 2.0f, &icons_config, ranges);

			io.Fonts->AddFontFromMemoryCompressedTTF(RobotoRegular_compressed_data, RobotoRegular_compressed_size, m_fontSize * 0.8f, &icons_config, ranges);
			AddIconFont();

			io.Fonts->AddFontDefault();
			AddIconFont();

			io.Fonts->TexGlyphPadding = 1;
			for (int n = 0; n < io.Fonts->ConfigData.Size; n++)
			{
				ImFontConfig* font_config = (ImFontConfig*)&io.Fonts->ConfigData[n];
				font_config->RasterizerMultiply = 1.0f;
			}

			ImGuiStyle& style = ImGui::GetStyle();

			style.WindowPadding = ImVec2(5, 5);
			style.FramePadding = ImVec2(4, 4);
			style.ItemSpacing = ImVec2(6, 2);
			style.ItemInnerSpacing = ImVec2(2, 2);
			style.IndentSpacing = 6.0f;
			style.TouchExtraPadding = ImVec2(4, 4);

			style.ScrollbarSize = 10;

			style.WindowBorderSize = 0;
			style.ChildBorderSize = 1;
			style.PopupBorderSize = 3;
			style.FrameBorderSize = 0.0f;

			const int roundingAmount = 2;
			style.PopupRounding = roundingAmount;
			style.WindowRounding = roundingAmount;
			style.ChildRounding = 0;
			style.FrameRounding = roundingAmount;
			style.ScrollbarRounding = roundingAmount;
			style.GrabRounding = roundingAmount;
			style.WindowMinSize = ImVec2(200.0f, 200.0f);
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

#ifdef IMGUI_HAS_DOCK
			style.TabBorderSize = 1.0f;
			style.TabRounding = roundingAmount; // + 4;

			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = roundingAmount;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
#endif

			ImGuiUtils::SetTheme(ImGuiUtils::Theme::Dark);
		}

		void ImGuiManager::AddIconFont()
		{
			ImGuiIO& io = ImGui::GetIO();

			static const ImWchar icons_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
			ImFontConfig icons_config;
			// merge in icons from Font Awesome
			icons_config.MergeMode = true;
			icons_config.PixelSnapH = true;
			icons_config.GlyphOffset.y = 1.0f;
			icons_config.OversampleH = icons_config.OversampleV = 1;
			icons_config.GlyphMinAdvanceX = 4.0f;
			icons_config.SizePixels = 12.0f;

			io.Fonts->AddFontFromMemoryCompressedTTF(MaterialDesign_compressed_data, MaterialDesign_compressed_size, m_fontSize, &icons_config, icons_ranges);
		}


	}
}