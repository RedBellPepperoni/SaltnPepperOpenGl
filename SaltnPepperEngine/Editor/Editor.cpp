#include "Editor.h"
#include "Engine/Core/System/Events/WindowEvent.h"
#include "Windows/InspectorWindow.h"
#include "Windows/HierarchyWindow.h"
#include <imgui/imgui_internal.h>

#include "Engine/Utils/Ray.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/System/Input/InputSystem.h"

#include "ImGuiManager.h"

namespace SaltnPepperEngine
{

	namespace Editor
	{
		RuntimeEditor::RuntimeEditor()
		{

		}

		RuntimeEditor::~RuntimeEditor()
		{
		}

		void RuntimeEditor::OnInit()
		{
			// Check on this later
			//m_editorCamera = MakeShared<Camera>();
			//m_currentCamera = m_editorCamera.get();

			m_editorWindows.emplace_back(MakeShared<InspectorWindow>());
			m_editorWindows.emplace_back(MakeShared<HierarchyWindow>());

			for (SharedPtr<EditorWindow> window : m_editorWindows)
			{
				window->SetEditor(this);
			}
		}

		void RuntimeEditor::OnQuit()
		{
			// Release all the editor windows
			for (SharedPtr<EditorWindow> eWindow : m_editorWindows)
			{
				eWindow->Release();
			}

			// Clear the container
			m_editorWindows.clear();
		}

	


		void RuntimeEditor::OnImGui()
		{
			// Work on this later
			DrawMenuBar();


			ImGui::Begin("Hello");

			ImGui::End();

			BeginDockSpace(m_properties.m_fullscreenLaunch && m_editorState == EditorState::Play);

			for (SharedPtr<EditorWindow> window : m_editorWindows)
			{
				if (window->GetActive())
				{
					window->OnImgui();
				}
			}

			m_properties.m_view2D = false;

			ImGuiIO& io = ImGui::GetIO();

			if (m_editorState == EditorState::Preview)
			{
				Application::GetCurrent().GetCurrentScene()->UpdateSceneGraph();
			}

			EndDockSpace();

			ImGuiDockContext* dc = &ImGui::GetCurrentContext()->DockContext;
			for (int n = 0; n < dc->Nodes.Data.Size; n++)
				if (ImGuiDockNode* node = (ImGuiDockNode*)dc->Nodes.Data[n].val_p)
				{
					if (node->TabBar)
						for (int n = 0; n < node->TabBar->Tabs.Size; n++)
						{
							const bool tab_visible = node->TabBar->VisibleTabId == node->TabBar->Tabs[n].ID;
							const bool tab_bar_focused = (node->TabBar->Flags & ImGuiTabBarFlags_IsFocused) != 0;
							if (tab_bar_focused || tab_visible)
							{
								auto tab = node->TabBar->Tabs[n];

								ImVec2 pos = tab.Window->Pos;
								pos.x = pos.x + tab.Offset + ImGui::GetStyle().FramePadding.x;
								pos.y = pos.y + ImGui::GetStyle().ItemSpacing.y;
								ImRect bb(pos, { pos.x + tab.ContentWidth, pos.y });

								tab.Window->DrawList->AddLine(bb.Min, bb.Max, (!tab_bar_focused) ? ImGui::GetColorU32(ImGuiCol_SliderGrabActive) : ImGui::GetColorU32(ImGuiCol_Text), 2.0f);
							}
						}
				}

		}

		void RuntimeEditor::OnUpdate()
		{
		}

		void RuntimeEditor::OnDebugDraw()
		{
		}

		void RuntimeEditor::SetEditorActive(bool active)
		{
			m_editorActive = active;
		}

		bool& RuntimeEditor::ToggleEditor()
		{
			m_editorActive = !m_editorActive;
			return m_editorActive;
		}

		void RuntimeEditor::BeginDockSpace(bool fullScrenePlay)
		{
			static bool windowOpen = true;
			static bool optionalFullscreenPersistant = true;

			static ImGuiDockNodeFlags optionalFlags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
		
			bool optionalFullscreen = optionalFullscreenPersistant;
		
			// making no docking since it would mess having 2 docking tagets within eatch other
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
			
			if (optionalFullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();

				ImVec2 pos = viewport->Pos;
				ImVec2 size = viewport->Size;
				bool menuBar = true;

				if (menuBar)
				{
					const float infoBarSize = ImGui::GetFrameHeight();
					pos.y += infoBarSize;
					size.y -= infoBarSize;
				}

				ImGui::SetNextWindowPos(pos);
				ImGui::SetNextWindowSize(size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
					| ImGuiWindowFlags_NoMove;
				windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
		
		
			if (optionalFlags & ImGuiDockNodeFlags_DockSpace)
			{
				windowFlags |= ImGuiWindowFlags_NoBackground;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			
			// =================== BEGIN THE DOCK SPACE UI ================
			ImGui::Begin("MainDockSpace", &windowOpen, windowFlags);
			ImGui::PopStyleVar();

			if (optionalFullscreen)
			{
				ImGui::PopStyleVar(2);
			}

			ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");

			static std::vector<SharedPtr<EditorWindow>> hiddenWindows;


			if (m_properties.m_fullscreenScene != fullScrenePlay)
			{
				m_properties.m_fullscreenScene = fullScrenePlay;

				if (m_properties.m_fullscreenScene)
				{
					// Hide all the other windows excpet the Game window
					for (SharedPtr<EditorWindow> window : m_editorWindows)
					{
						if (window->GetName() != "Game" && window->GetActive())
						{
							window->SetActive(false);
							hiddenWindows.push_back(window);
						}
					}
				}

				else
				{
					// Un-Hide all the previous active windows
					for (SharedPtr<EditorWindow> window : hiddenWindows)
					{
						window->SetActive(true);
					}

					// Clearing the container to avoid issues
					hiddenWindows.clear();

				}

				if (!ImGui::DockBuilderGetNode(dockspaceID))
				{
					ImGui::DockBuilderRemoveNode(dockspaceID); // Clear out existing layout
					ImGui::DockBuilderAddNode(dockspaceID);    // Add empty node
					ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetIO().DisplaySize * ImGui::GetIO().DisplayFramebufferScale);

					ImGuiID dock_main_id = dockspaceID;
					ImGuiID DockBottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.3f, nullptr, &dock_main_id);
					ImGuiID DockLeft = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
					ImGuiID DockRight = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);

					ImGuiID DockLeftChild = ImGui::DockBuilderSplitNode(DockLeft, ImGuiDir_Down, 0.875f, nullptr, &DockLeft);
					ImGuiID DockRightChild = ImGui::DockBuilderSplitNode(DockRight, ImGuiDir_Down, 0.875f, nullptr, &DockRight);
					ImGuiID DockingLeftDownChild = ImGui::DockBuilderSplitNode(DockLeftChild, ImGuiDir_Down, 0.06f, nullptr, &DockLeftChild);
					ImGuiID DockingRightDownChild = ImGui::DockBuilderSplitNode(DockRightChild, ImGuiDir_Down, 0.06f, nullptr, &DockRightChild);

					ImGuiID DockBottomChild = ImGui::DockBuilderSplitNode(DockBottom, ImGuiDir_Down, 0.2f, nullptr, &DockBottom);
					ImGuiID DockingBottomLeftChild = ImGui::DockBuilderSplitNode(DockLeft, ImGuiDir_Down, 0.4f, nullptr, &DockLeft);
					ImGuiID DockingBottomRightChild = ImGui::DockBuilderSplitNode(DockRight, ImGuiDir_Down, 0.4f, nullptr, &DockRight);

					ImGuiID DockMiddle = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.8f, nullptr, &dock_main_id);
					ImGuiID DockBottomMiddle = ImGui::DockBuilderSplitNode(DockMiddle, ImGuiDir_Down, 0.3f, nullptr, &DockMiddle);
					ImGuiID DockMiddleLeft = ImGui::DockBuilderSplitNode(DockMiddle, ImGuiDir_Left, 0.5f, nullptr, &DockMiddle);
					ImGuiID DockMiddleRight = ImGui::DockBuilderSplitNode(DockMiddle, ImGuiDir_Right, 0.5f, nullptr, &DockMiddle);

					ImGui::DockBuilderDockWindow("Game", DockMiddleRight);
					ImGui::DockBuilderDockWindow("Scene", DockMiddleLeft);
					ImGui::DockBuilderDockWindow("Inspector", DockRight);
					ImGui::DockBuilderDockWindow("Console", DockBottomMiddle);
					ImGui::DockBuilderDockWindow("Profiler", DockingBottomLeftChild);
					ImGui::DockBuilderDockWindow("ExtraWindow", DockLeft);
					ImGui::DockBuilderDockWindow("GraphicsInfo", DockLeft);
					ImGui::DockBuilderDockWindow("ApplicationInfo", DockLeft);
					ImGui::DockBuilderDockWindow("Hierarchy", DockLeft);
					ImGui::DockBuilderDockWindow("TextEdit", DockMiddleLeft);
					ImGui::DockBuilderDockWindow("Scenesettings", DockLeft);
					ImGui::DockBuilderDockWindow("Editorsettings", DockLeft);
					ImGui::DockBuilderDockWindow("Projectsettings", DockLeft);

					ImGui::DockBuilderFinish(dockspaceID);

				}
			}
		
		}

		void RuntimeEditor::EndDockSpace()
		{
			ImGui::End();
		}

		void RuntimeEditor::DrawMenuBar()
		{
		}

		bool& RuntimeEditor::ShowGizmos()
		{
			return m_properties.m_showGizmos;
		}

		bool& RuntimeEditor::ShowViewSelected()
		{
			return m_properties.m_showViewSelected;
		}

		bool& RuntimeEditor::FullScreenOnLaunch()
		{
			return m_properties.m_fullscreenLaunch;
		}

		void RuntimeEditor::SelectEntity(entt::entity entity)
		{
			entt::registry& registry = Application::GetCurrent().GetCurrentScene()->GetRegistry();
		

			// Check if the entity is actually valid is doesnt already exists in the selected list
			if (!registry.valid(entity))
			{
				return;
			}
			if (std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) != m_selectedEntities.end())
			{
				return;
			}

			// Add the enitiy to the selected list
			m_selectedEntities.push_back(entity);
		
		}

		void RuntimeEditor::UnselectEntity(entt::entity entity)
		{
			std::vector<entt::entity>::iterator iterator = std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity);
		
			// If the given entity is in the selected list clear it
			if (iterator != m_selectedEntities.end())
			{
				m_selectedEntities.erase(iterator);
			}
		}

		void RuntimeEditor::ClearSelectedEntity()
		{
			m_selectedEntities.clear();
		}

		const std::vector<entt::entity>& RuntimeEditor::GetSelectedEntities() const
		{
			return m_selectedEntities;
		}

		bool RuntimeEditor::IsEntitySelected(entt::entity entity)
		{
			if (std::find(m_selectedEntities.begin(), m_selectedEntities.end(), entity) != m_selectedEntities.end())
			{
				return true;
			}

			return false;
		}

		void RuntimeEditor::FocusCamera(const Vector3& targetpoint, float distance, float speed)
		{
			if (m_currentCamera->IsOrthographic())
			{
				// Not focusing on Ortho camera right now so empty
			}

			// Camera is perspective
			else
			{
				m_camerainTransition = true;

				m_cameraDestition = targetpoint + m_editorCameraTransform.GetForwardVector() * distance;
				m_cameraTrasitionStartTime = -1.0f;
				m_cameraTransitionSpeed = 1.0f / speed;
				m_cameraOrigin = m_editorCameraTransform.GetPosition();
			}
		}

		void RuntimeEditor::ShowPreview()
		{
			ImGui::Begin("Preview");

			if (m_previewTexture)
			{
				ImGuiUtils::Image(m_previewTexture.get(), { 200, 200 });
			}

			ImGui::End();
		}

		void RuntimeEditor::DrawPreview()
		{
			// Focus on this later
			if (!m_previewTexture)
			{

			}
		}

		void RuntimeEditor::SelectObjectRay(const Ray& ray)
		{
		}

		EditorWindow* RuntimeEditor::GetTextEditWindow()
		{
			return nullptr;
		}

		void RuntimeEditor::RemoveEditorWindow(EditorWindow* window)
		{
		}

		Ray RuntimeEditor::GetScreenRay(uint32_t xPos, uint32_t yPos, Camera* camera, uint32_t width, uint32_t height)
		{
			if (!camera)
				return Ray();


			float screenX = (float)xPos / (float)width;
			float screenY = (float)yPos / (float)width;

			bool flipY = true;

			return camera->GetRay(screenX, screenY, Inverse(m_editorCameraTransform.GetMatrix()), flipY);

		}

		void RuntimeEditor::ActivateSceneView(bool active)
		{
		}

		bool RuntimeEditor::OnWindowResize(WindowResizeEvent& event)
		{
			return false;
		}
	}
}
