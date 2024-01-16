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
			: m_windowHandle(nullptr)
		{
			m_clearScreen = clearScreen;
		}

		ImGuiRenderer::~ImGuiRenderer()
		{
			ImGui_ImplOpenGL3_Shutdown();
		}

		void ImGuiRenderer::Init()
		{
			
			ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, true);

			ImGui_ImplOpenGL3_Init();
			ImGui_ImplOpenGL3_NewFrame();
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

			/*if (m_clearScreen)
			{
				GLDEBUG(glClear(GL_COLOR_BUFFER_BIT));
			}*/

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void ImGuiRenderer::OnResize(uint32_t width, uint32_t height)
		{
			// Implement later
		}
		void ImGuiRenderer::SetWindowRef(GLFWwindow* windowRef)
		{
			m_windowHandle = windowRef;
		}
	}
}