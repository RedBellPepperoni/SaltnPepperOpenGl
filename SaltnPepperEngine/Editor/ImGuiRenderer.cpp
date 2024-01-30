#include "ImGuiRenderer.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_opengl3.h>
#include "Engine/Utils/GLUtils.h"
#include <Imgui/imgui_impl_glfw.h>

namespace SaltnPepperEngine
{
	namespace Editor
	{
		ImGuiRenderer::ImGuiRenderer(bool clearScreen)
		{
			m_clearScreen = clearScreen;
		}

		ImGuiRenderer::~ImGuiRenderer()
		{
			ImGui_ImplOpenGL3_Shutdown();
		}

		void ImGuiRenderer::Init(GLFWwindow* windowHandle)
		{		
			/*ImGuiContext* context = ImGui::CreateContext();
			ImGui::SetCurrentContext(context);

			ImGuiIO& imguiIO = ImGui::GetIO();

			imguiIO.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
			imguiIO.ConfigDockingAlwaysTabBar = true;*/



			ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);
			ImGui_ImplOpenGL3_Init();
			ImGui::StyleColorsDark();

			//NewFrame();

		}

		void ImGuiRenderer::NewFrame()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void ImGuiRenderer::Render()
		{
			ImGui::Render();

			if (m_clearScreen)
			{
				GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
			}

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void ImGuiRenderer::OnResize(uint32_t width, uint32_t height)
		{
			// Implement later
		}
	}
}