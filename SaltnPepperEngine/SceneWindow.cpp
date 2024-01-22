#include "SceneWindow.h"
#include "Editor/MDIcons.h"

#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Camera/Camera.h"

#include "Engine/Core/System/Application/Application.h"
#include "Editor/Editor.h"
#include "Imgui/ImGuizmo.h"

namespace SaltnPepperEngine
{
	namespace Editor
	{
		SceneWindow::SceneWindow()
		{
			m_name = ICON_MDI_GAMEPAD_VARIANT " Scene###scene";
			m_CurrentScene = nullptr;

			m_ShowComponentGizmoMap[typeid(Light).hash_code()] = true;
			m_ShowComponentGizmoMap[typeid(Camera).hash_code()] = true;
			
			m_Width = 1920;
			m_Height = 1200;

		}

		void SceneWindow::OnInit()
		{
			Application& app = Application::GetCurrent();

			ImGuiUtils::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

			auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

			if (!ImGui::Begin(m_name.c_str(), &m_active, flags) || !m_CurrentScene)
			{	
				ImGui::End();
				return;
			}

			Camera* camera = nullptr;
			Transform* transform = nullptr;
				
			camera = m_editorHandle->GetCamera();
			transform = &m_editorHandle->GetEditorCameraTransform();


			ImVec2 offset = { 0.0f, 0.0f };

			{
				ToolBar();
				offset = ImGui::GetCursorPos(); 
			}

			if (!camera)
			{
				ImGui::End();
				return;
			}

			ImGuizmo::SetDrawlist();
			auto sceneViewSize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin() - offset * 0.5f; // - offset * 0.5f;
			auto sceneViewPosition = ImGui::GetWindowPos() + offset;

			sceneViewSize.x -= static_cast<int>(sceneViewSize.x) % 2 != 0 ? 1.0f : 0.0f;
			sceneViewSize.y -= static_cast<int>(sceneViewSize.y) % 2 != 0 ? 1.0f : 0.0f;

			float aspect = static_cast<float>(sceneViewSize.x) / static_cast<float>(sceneViewSize.y);

			if (!FloatEquals(aspect, camera->GetAspectRatio()))
			{
				camera->SetAspectRatio(aspect);
			}

			m_editorHandle->m_sceneViewPosition = glm::vec2(sceneViewPosition.x, sceneViewPosition.y);

			if (m_editorHandle->GetProperties().m_halfRes)
			{
				sceneViewSize *= 0.5f;
			}

			sceneViewSize.x -= static_cast<int>(sceneViewSize.x) % 2 != 0 ? 1.0f : 0.0f;
			sceneViewSize.y -= static_cast<int>(sceneViewSize.y) % 2 != 0 ? 1.0f : 0.0f;

			Resize(static_cast<uint32_t>(sceneViewSize.x), static_cast<uint32_t>(sceneViewSize.y));

			if (m_editorHandle->GetProperties().m_halfRes)
				sceneViewSize *= 2.0f;

			ImGuiUtils::Image(m_GameViewTexture.get(), glm::vec2(sceneViewSize.x, sceneViewSize.y));

			auto windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = sceneViewPosition;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
			bool updateCamera = ImGui::IsMouseHoveringRect(minBound, maxBound); 

			m_editorHandle->SetSceneActive(ImGui::IsWindowFocused() && !ImGuizmo::IsUsing() && updateCamera);

			ImGuizmo::SetRect(sceneViewPosition.x, sceneViewPosition.y, sceneViewSize.x, sceneViewSize.y);

			m_editorHandle->ActivateSceneView(updateCamera);
			{
				ImGui::GetWindowDrawList()->PushClipRect(sceneViewPosition, { sceneViewSize.x + sceneViewPosition.x, sceneViewSize.y + sceneViewPosition.y - 2.0f });
			}


		}

		void SceneWindow::OnImgui()
		{
		}

		void SceneWindow::ToolBar()
		{
		}

		void SceneWindow::Resize(uint32_t width, uint32_t height)
		{
		}
	}
}
