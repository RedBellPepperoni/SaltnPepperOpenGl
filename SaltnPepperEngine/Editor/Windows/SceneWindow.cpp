#include "SceneWindow.h"
#include "Editor/MDIcons.h"

#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Camera/Camera.h"

#include "Engine/Core/System/Application/Application.h"
#include "Editor/Editor.h"
#include "Imgui/ImGuizmo.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Utils/Ray.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Rendering/Renderer/Renderer.h"
#include "Engine/Core/Rendering/Renderer/RenderManager.h"

namespace SaltnPepperEngine
{
	namespace Editor
	{
		SceneWindow::SceneWindow()
		{
			m_name = ICON_MDI_GAMEPAD_VARIANT " Scene###scene";
			m_CurrentScene = nullptr;

			m_ShowComponentGizmoMap[typeid(LightComponent).hash_code()] = true;
			m_ShowComponentGizmoMap[typeid(Camera).hash_code()] = true;
			
			m_Width = 1920;
			m_Height = 1200;
	

		}

		void SceneWindow::OnInit()
		{
			m_GameViewTexture = Application::GetCurrent().GetEditorCamera()->GetRenderTexture();
			
		}

		void SceneWindow::OnImgui()
		{
			Application& app = Application::GetCurrent();

			ImGuiUtils::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

			int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;


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
			ImVec2 sceneViewSize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin() - offset * 0.5f; // - offset * 0.5f;
			ImVec2 sceneViewPosition = ImGui::GetWindowPos() + offset;

			sceneViewSize.x -= static_cast<int>(sceneViewSize.x) % 2 != 0 ? 1.0f : 0.0f;
			sceneViewSize.y -= static_cast<int>(sceneViewSize.y) % 2 != 0 ? 1.0f : 0.0f;

			float aspect = static_cast<float>(sceneViewSize.x) / static_cast<float>(sceneViewSize.y);

			if (!FloatEquals(aspect, camera->GetAspectRatio()))
			{
				camera->SetAspectRatio(aspect);
			}

			m_editorHandle->m_sceneViewPosition = Vector2(sceneViewPosition.x, sceneViewPosition.y);

			if (m_editorHandle->GetProperties().m_halfRes)
			{
				sceneViewSize *= 0.5f;
			}

			sceneViewSize.x -= static_cast<int>(sceneViewSize.x) % 2 != 0 ? 1.0f : 0.0f;
			sceneViewSize.y -= static_cast<int>(sceneViewSize.y) % 2 != 0 ? 1.0f : 0.0f;

			Resize(static_cast<uint32_t>(sceneViewSize.x), static_cast<uint32_t>(sceneViewSize.y));

			if (m_editorHandle->GetProperties().m_halfRes)
				sceneViewSize *= 2.0f;



			//ImGuiUtils::Image(m_GameViewTexture.get(), glm::vec2(sceneViewSize.x, sceneViewSize.y));

			//Texture* texture = Application::GetCurrent().GetRenderManager()->GetRenderer()->SecondaryTexture.get();
			Texture* texture = Application::GetCurrent().GetEditorCamera()->GetRenderTexture().get();
			ImGuiUtils::Image(texture, glm::vec2(sceneViewSize.x, sceneViewSize.y));

			ImVec2 windowSize = ImGui::GetWindowSize();
			ImVec2 minBound = sceneViewPosition;

			ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

			//bool updateCamera = ImGui::IsMouseHoveringRect(minBound, maxBound);

			// Idiotic fix until , I fix the real Issue
			bool updateCamera = true;

			
			m_editorHandle->SetSceneActive(ImGui::IsWindowFocused() && !ImGuizmo::IsUsing() && updateCamera);
			//m_editorHandle->SetSceneActive(ImGui::IsWindowFocused() && !ImGuizmo::IsUsing());

			ImGuizmo::SetRect(sceneViewPosition.x, sceneViewPosition.y, sceneViewSize.x, sceneViewSize.y);

			m_editorHandle->ActivateSceneView(updateCamera);
			{
				ImGui::GetWindowDrawList()->PushClipRect(sceneViewPosition, { sceneViewSize.x + sceneViewPosition.x, sceneViewSize.y + sceneViewPosition.y - 2.0f });
			}

			if (m_editorHandle->ShowGrid())
			{
				if (camera->IsOrthographic())
				{
					// Draw 2D grid here
					//m_editorHandle->Draw2DGrid();
				}
			}

			m_editorHandle->OnImGuizmo();

			if (updateCamera && m_editorHandle->GetSceneActive() && !ImGuizmo::IsUsing() && InputSystem::GetInstance().GetMouseBtnClicked(Input::MouseButton::Left))
			{
				float dpi = 1.0f;
				auto clickPos = InputSystem::GetInstance().GetMousePosition() - glm::vec2(sceneViewPosition.x / dpi, sceneViewPosition.y / dpi);

				Ray ray = m_editorHandle->GetScreenRay(int(clickPos.x), int(clickPos.y), camera, int(sceneViewSize.x / dpi), int(sceneViewSize.y / dpi));
				m_editorHandle->SelectObjectRay(ray);
			}

			const ImGuiPayload* payload = ImGui::GetDragDropPayload();

			Scene* scene = Application::GetCurrent().GetCurrentScene();

			if (scene)
			{
				DrawGizmos(sceneViewSize.x, sceneViewSize.y, offset.x, offset.y, scene);
			}

			ImGui::GetWindowDrawList()->PopClipRect();
			ImGui::End();
		}

		void SceneWindow::ToolBar()
		{

		}

		void SceneWindow::Resize(uint32_t width, uint32_t height)
		{
			

		}

		void SceneWindow::DrawGizmos(float width, float height, float xPos, float yPos, Scene* scene)
		{
			Camera* camera = m_editorHandle->GetCamera();
			Transform& cameraTransform = m_editorHandle->GetEditorCameraTransform();
			entt::registry& registry = scene->GetRegistry();
			Matrix4 view = Inverse(cameraTransform.GetMatrix());
			Matrix4 proj = camera->GetProjectionMatrix();
			Matrix4 viewProj = proj * view;
			const Frustum& f = camera->GetFrustum(view);

			ShowComponentGizmo<LightComponent>(width, height, xPos, yPos, viewProj, f, registry);
			ShowComponentGizmo<Camera>(width, height, xPos, yPos, viewProj, f, registry);
			
		}

		void SceneWindow::OnNewScene(Scene* sceneRef)
		{
			m_CurrentScene = sceneRef;
			m_GameViewTexture = Application::GetCurrent().GetEditorCamera()->GetRenderTexture();

		}

	}
}
