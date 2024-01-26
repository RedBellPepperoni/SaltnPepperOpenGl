#include "Editor.h"
#include "Engine/Core/System/Events/WindowEvent.h"
#include "Windows/InspectorWindow.h"
#include "Windows/HierarchyWindow.h"
#include "Windows/SceneWindow.h"
#include <imgui/imgui_internal.h>

#include "Engine/Utils/Ray.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Editor/MDIcons.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "Engine/Core/Rendering/Geometry/Model.h"

#include "ImGuiManager.h"
#include "Imgui/ImGuizmo.h"
#include "Engine/Core/EntitySystem/Entity.h"

namespace SaltnPepperEngine
{
	using namespace Physics;

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

			ImGuizmo::Style& guizmoStyle = ImGuizmo::GetStyle();
			guizmoStyle.HatchedAxisLineThickness = -1.0f;
			ImGuizmo::SetGizmoSizeClipSpace(m_properties.m_imGuizmoScale);

			// Create a new Editor camera
			m_editorCamera = MakeShared<Camera>((16.0f / 10.0f), 0.01f, 1000.0f);
			m_currentCamera = m_editorCamera.get();
			// Set the initial  postion
			m_editorCameraTransform.SetPosition(Vector3(0.0f,0.0f,-40.0f));
			m_editorCameraTransform.SetMatrix(Matrix4(1.0f));

			// Set the camera the controoler will access
			m_editorCameraController.SetCamera(m_editorCamera.get());

			m_componentIconMap[typeid(Light).hash_code()] = ICON_MDI_LIGHTBULB;
			m_componentIconMap[typeid(Camera).hash_code()] = ICON_MDI_CAMERA;
			m_componentIconMap[typeid(Transform).hash_code()] = ICON_MDI_VECTOR_LINE;
			m_componentIconMap[typeid(RigidBody3D).hash_code()] = ICON_MDI_CUBE_OUTLINE;
			m_componentIconMap[typeid(ModelComponent).hash_code()] = ICON_MDI_VECTOR_POLYGON;
			m_componentIconMap[typeid(Model).hash_code()] = ICON_MDI_VECTOR_POLYGON;
			m_componentIconMap[typeid(RuntimeEditor).hash_code()] = ICON_MDI_SQUARE;
			

			SharedPtr<InspectorWindow> window = MakeShared<InspectorWindow>();
			
			m_editorWindows.emplace_back(window);
			m_editorWindows.emplace_back(MakeShared<HierarchyWindow>());

			SharedPtr<SceneWindow> sceneWindow = MakeShared<SceneWindow>();
			
			m_editorWindows.emplace_back(sceneWindow);


			for (SharedPtr<EditorWindow> windows : m_editorWindows)
			{
				windows->SetEditor(this);
			}

			window->OnInit();

			sceneWindow->OnInit();
			sceneWindow->OnNewScene(Application::GetCurrent().GetCurrentScene());

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
			//DrawMenuBar();

			BeginDockSpace(m_properties.m_fullscreenLaunch && m_editorState == EditorState::Play);

			for (SharedPtr<EditorWindow> window : m_editorWindows)
			{
				if (window->GetActive())
				{
					window->OnImgui();
				}
			}

			m_properties.m_view2D = m_currentCamera->IsOrthographic();

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
								ImGuiTabItem tab = node->TabBar->Tabs[n];

								ImVec2 pos = tab.Window->Pos;
								pos.x = pos.x + tab.Offset + ImGui::GetStyle().FramePadding.x;
								pos.y = pos.y + ImGui::GetStyle().ItemSpacing.y;
								ImRect bb(pos, { pos.x + tab.ContentWidth, pos.y });

								tab.Window->DrawList->AddLine(bb.Min, bb.Max, (!tab_bar_focused) ? ImGui::GetColorU32(ImGuiCol_SliderGrabActive) : ImGui::GetColorU32(ImGuiCol_Text), 2.0f);
							}
						}
				}

		}

		void RuntimeEditor::OnImGuizmo()
		{
			Matrix4 view = Inverse(m_editorCameraTransform.GetMatrix());
			Matrix4 proj = m_currentCamera->GetProjectionMatrix();


			if (!m_properties.m_showGizmos || m_selectedEntities.empty() || m_imGuizmoOperation == 4)
				return;

			entt::registry& registry = Application::GetCurrent().GetCurrentScene()->GetRegistry();

			if (m_selectedEntities.size() == 1)
			{
				entt::entity m_SelectedEntity = entt::null;

				m_SelectedEntity = m_selectedEntities.front();
				if (registry.valid(m_SelectedEntity))
				{
					ImGuizmo::SetDrawlist();
					ImGuizmo::SetOrthographic(m_currentCamera->IsOrthographic());

					Transform* transform = registry.try_get<Transform>(m_SelectedEntity);
					if (transform != nullptr)
					{
						Matrix4 model = transform->GetMatrix();

						float snapAmount[3] = { m_properties.m_snapAmount, m_properties.m_snapAmount, m_properties.m_snapAmount };
						float delta[16];

						ImGuizmo::Manipulate(glm::value_ptr(view),
							glm::value_ptr(proj),
							static_cast<ImGuizmo::OPERATION>(m_imGuizmoOperation),
							ImGuizmo::LOCAL,
							glm::value_ptr(model),
							delta,
							m_properties.m_snapQuizmo ? snapAmount : nullptr);

						if (ImGuizmo::IsUsing())
						{
							Entity parent = Entity(m_SelectedEntity, Application::GetCurrent().GetCurrentScene()).GetParent(); // m_CurrentScene->TryGetEntityWithUUID(entity.GetParentUUID());
							
							if (parent && parent.HasComponent<Transform>())
							{
								Matrix4 parentTransform = parent.GetTransform().GetMatrix();
								model = Inverse(parentTransform) * model;
							}

							if (ImGuizmo::IsScaleType()) // static_cast<ImGuizmo::OPERATION>(m_ImGuizmoOperation) & ImGuizmo::OPERATION::SCALE)
							{
								transform->SetScale(GetScaleFromMatrix(model));
							}
							else
							{
								transform->SetLocalMatrix(model);

								RigidBody3D* rigidBody3DComponent = registry.try_get<RigidBody3D>(m_SelectedEntity);
								if (rigidBody3DComponent)
								{
									rigidBody3DComponent->SetPosition(model[3]);
									rigidBody3DComponent->SetRotation(GetRotationFromMatrix(model));
								}		
								
							}
						}
					}
				}
			}
			else
			{
				Vector3 medianPointLocation = Vector3(0.0f);
				Vector3 medianPointScale = Vector3(0.0f);
				int validcount = 0;
				for (auto entityID : m_selectedEntities)
				{
					if (!registry.valid(entityID))
						continue;

					Entity entity = { entityID, Application::GetCurrent().GetCurrentScene()};

					if (!entity.HasComponent<Transform>())
						continue;

					medianPointLocation += entity.GetTransform().GetWorldPosition();
					medianPointScale += entity.GetTransform().GetScale();
					validcount++;
				}
				medianPointLocation /= validcount; // m_SelectedEntities.size();
				medianPointScale /= validcount;    // m_SelectedEntities.size();

				Matrix4 medianPointMatrix = Translate(Matrix4(1.0f), medianPointLocation) * Scale(Matrix4(1.0f), medianPointScale);

				ImGuizmo::SetDrawlist();
				ImGuizmo::SetOrthographic(m_currentCamera->IsOrthographic());

				float snapAmount[3] = { m_properties.m_snapAmount, m_properties.m_snapAmount, m_properties.m_snapAmount };
				Matrix4 deltaMatrix = Matrix4(1.0f);

				ImGuizmo::Manipulate(glm::value_ptr(view),
					glm::value_ptr(proj),

					static_cast<ImGuizmo::OPERATION>(m_imGuizmoOperation),
					ImGuizmo::LOCAL,
					glm::value_ptr(medianPointMatrix),
					glm::value_ptr(deltaMatrix),
					m_properties.m_snapQuizmo ? snapAmount : nullptr);

				if (ImGuizmo::IsUsing())
				{
					Vector3 deltaTranslation, deltaScale;
					Quaternion deltaRotation;
					Vector3 skew;
					Vector4 perspective;
					glm::decompose(deltaMatrix, deltaScale, deltaRotation, deltaTranslation, skew, perspective);

					
					static const bool MedianPointOrigin = false;

					if (MedianPointOrigin)
					{
						for (auto entityID : m_selectedEntities)
						{
							if (!registry.valid(entityID))
								continue;
							auto transform = registry.try_get<Transform>(entityID);

							if (!transform)
								continue;
							if (ImGuizmo::IsScaleType()) // static_cast<ImGuizmo::OPERATION>(m_ImGuizmoOperation) == ImGuizmo::OPERATION::SCALE)
							{
								transform->SetScale(transform->GetScale() * deltaScale);
								// transform->SetLocalTransform(deltaMatrix * transform->GetLocalMatrix());
							}
							else
							{
								transform->SetLocalMatrix(deltaMatrix * transform->GetLocalMatrix());

								// World matrix wont have updated yet so need to multiply by delta
								// TODO: refactor
								auto worldMatrix = deltaMatrix * transform->GetMatrix();

								
								RigidBody3D* rigidBody3DComponent = registry.try_get<RigidBody3D>(entityID);
								if (rigidBody3DComponent)
								{
									rigidBody3DComponent->SetPosition(worldMatrix[3]);
									rigidBody3DComponent->SetRotation(GetRotationFromMatrix(worldMatrix));
								}
								
								
									
								
							}
						}
					}
					else
					{
						for (auto entityID : m_selectedEntities)
						{
							if (!registry.valid(entityID))
								continue;
							auto transform = registry.try_get<Transform>(entityID);

							if (!transform)
								continue;
							if (ImGuizmo::IsScaleType()) // static_cast<ImGuizmo::OPERATION>(m_ImGuizmoOperation) & ImGuizmo::OPERATION::SCALE)
							{
								transform->SetScale(transform->GetScale() * deltaScale);
								// transform->SetLocalTransform(deltaMatrix * transform->GetLocalMatrix());
							}
							else if (ImGuizmo::IsRotateType()) // static_cast<ImGuizmo::OPERATION>(m_ImGuizmoOperation) & ImGuizmo::OPERATION::ROTATE)
							{
								transform->SetRotation(Quaternion(GetEularAnglesRadians(transform->GetRotation()) + GetEularAnglesRadians(deltaRotation)));
							}
							else
							{
								transform->SetLocalMatrix(deltaMatrix * transform->GetLocalMatrix());

								// World matrix wont have updated yet so need to multiply by delta
								// TODO: refactor
								Matrix4 worldMatrix = deltaMatrix * transform->GetMatrix();

								RigidBody3D* rigidBody3DComponent = registry.try_get<RigidBody3D>(entityID);
								if (rigidBody3DComponent)
								{
									rigidBody3DComponent->SetPosition(worldMatrix[3]);
									rigidBody3DComponent->SetRotation(GetRotationFromMatrix(worldMatrix));
								}
									
								
							}
						}
					}
				}
			}

		}

		void RuntimeEditor::OnUpdate(float deltaTime)
		{
			if (InputSystem::GetInstance().GetKeyDown(Key::Delete))
			{
				auto* scene = Application::GetCurrent().GetCurrentScene();
				for (auto entity : m_selectedEntities)
				{
					scene->DestroyEntity(Entity(entity, scene));
				}
			}

			if (m_sceneViewActive)
			{
				entt::registry& registry = Application::GetCurrent().GetCurrentScene()->GetRegistry();

				// if(Application::Get().GetSceneActive())
				{
					const Vector2 mousePos = InputSystem::GetInstance().GetMousePosition();
					//m_editorCameraController.SetCamera(m_EditorCamera);
					m_editorCameraController.MouseInput(m_editorCameraTransform, mousePos,deltaTime);
					m_editorCameraController.KeyboardInput(m_editorCameraTransform, deltaTime);
					m_editorCameraTransform.SetMatrix(Matrix4(1.0f));

					if (!m_selectedEntities.empty() && InputSystem::GetInstance().GetKeyDown(Key::F))
					{
						if (registry.valid(m_selectedEntities.front()))
						{
							Transform* transform = registry.try_get<Transform>(m_selectedEntities.front());
							if (transform)
							{
								FocusCamera(transform->GetWorldPosition(), 2.0f, 2.0f);
							}
						}
					}
				}

				if (InputSystem::GetInstance().GetKeyHeld(Key::O))
				{
					FocusCamera(Vector3(0.0f, 0.0f, 0.0f), 2.0f, 2.0f);
				}

				if (m_camerainTransition)
				{
					if (m_cameraTransitionStartTime < 0.0f)
					{
						m_cameraTransitionStartTime = Application::GetCurrent().GetTotalElapsed();
					}

					float focusProgress = Min(((Application::GetCurrent().GetTotalElapsed() - m_cameraTransitionStartTime) / m_cameraTransitionSpeed), 1.0f);
					Vector3 newCameraPosition = glm::mix(m_cameraOrigin, m_cameraDestination, focusProgress);
					m_editorCameraTransform.SetPosition(newCameraPosition);

					if (m_editorCameraTransform.GetPosition() == m_cameraDestination)
					{
						m_camerainTransition = false;
					}
				}

				if (!InputSystem::GetInstance().GetMouseBtnHeld(MouseButton::Right) && !ImGuizmo::IsUsing())
				{
					if (InputSystem::GetInstance().GetKeyDown(Key::Q))
					{
						SetImGuizmoOperation(ImGuizmo::OPERATION::BOUNDS);
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::W))
					{
						SetImGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::E))
					{
						SetImGuizmoOperation(ImGuizmo::OPERATION::ROTATE);
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::R))
					{
						SetImGuizmoOperation(ImGuizmo::OPERATION::SCALE);
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::T))
					{
						SetImGuizmoOperation(ImGuizmo::OPERATION::UNIVERSAL);
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::Y))
					{
						ToggleSnap();
					}
				}

				if ((InputSystem::GetInstance().GetKeyHeld(Key::LeftControl)))
				{
					

					if (InputSystem::GetInstance().GetKeyDown(Key::X))
					{
						for (entt::entity entity : m_selectedEntities)
						{
							SetCopiedEntity(entity, true);
						}
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::C))
					{
						for (entt::entity entity : m_selectedEntities)
						{
							SetCopiedEntity(entity, false);
						}
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::V) && !m_copiedEntities.empty())
					{
						for (entt::entity entity : m_copiedEntities)
						{
							Application::GetCurrent().GetCurrentScene()->Duplicate({ entity, Application::GetCurrent().GetCurrentScene() });
							if (entity != entt::null)
							{
								
								Entity(entity, Application::GetCurrent().GetCurrentScene()).Destroy();
							}
						}
					}

					if (InputSystem::GetInstance().GetKeyDown(Key::D) && !m_selectedEntities.empty())
					{
						for (entt::entity entity : m_copiedEntities)
						{
							Application::GetCurrent().GetCurrentScene()->Duplicate({ entity, Application::GetCurrent().GetCurrentScene() });
						}
					}
				}
			}
			else
			{
				m_editorCameraController.StopMovement();
			}


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
			//ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;;
			
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


			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{	
				ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optionalFlags);
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

		void RuntimeEditor::ToggleSnap()
		{
			m_properties.m_snapQuizmo = !m_properties.m_snapQuizmo;
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

		bool RuntimeEditor::IsCopied(entt::entity entity)
		{
			if (std::find(m_copiedEntities.begin(), m_copiedEntities.end(), entity) != m_copiedEntities.end())
			{
				return true;
			}

			return false;
		}

		void RuntimeEditor::SetCopiedEntity(entt::entity entity, bool cut)
		{
			if (std::find(m_copiedEntities.begin(), m_copiedEntities.end(), entity) != m_copiedEntities.end())
				return;

			m_copiedEntities.push_back(entity);
			m_cutCopyEntity = cut;
		}

		const std::vector<entt::entity>& RuntimeEditor::GetCopiedEntity() const
		{
			return m_copiedEntities;
		}

		bool RuntimeEditor::GetCutCopyEntity()
		{
			return m_cutCopyEntity;
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

				m_cameraDestination = targetpoint + m_editorCameraTransform.GetForwardVector() * distance;
				m_cameraTransitionStartTime = -1.0f;
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

		void RuntimeEditor::SetSceneViewSize(uint32_t width, uint32_t height)
		{
			if (width != m_sceneViewWidth)
			{
				m_sceneViewWidth = width;
				m_sceneViewSizeUpdated = true;
			}

			if (height != m_sceneViewHeight)
			{
				m_sceneViewHeight = height;
				m_sceneViewSizeUpdated = true;
			}
		}

		std::unordered_map<size_t, const char*>& RuntimeEditor::GetComponentIconMap()
		{
			return m_componentIconMap;
		}

		bool RuntimeEditor::OnWindowResize(WindowResizeEvent& event)
		{
			return false;
		}
	}
}
