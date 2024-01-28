#ifndef EDITOR_H
#define EDITOR_H

#include "EditorWindow.h"
#include "Editor/Windows/FileBrowserWindow.h"
#include "ImGuiUtils.h"

#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Rendering/Camera/EditorCameraController.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/Transform.h"


#include <imgui/imgui.h>
#include <entt/entt.hpp>



namespace SaltnPepperEngine
{
	class Scene;
	class Event;
	class WindowResizeEvent;
	class WindowCloseEvent;
	class Ray;
	
	namespace Rendering
	{
		class Texture;
		class Mesh;
	}
	
	using namespace Rendering;

	using namespace Components;

	namespace Editor
	{
		class ImGuiManager;

		enum class EditorState
		{
			Paused,
			Play,
			Next,
			Preview
		};



		class RuntimeEditor
		{
		public: 

			RuntimeEditor();
			~RuntimeEditor();

			void OnInit();
			void OnQuit();


			void OnImGui();
			void OnImGuizmo();

			void OnUpdate(float deltaTime);
			void OnDebugDraw();

			void SetEditorActive(bool active);
			bool IsEditorActive();
			bool& ToggleEditor();

			void BeginDockSpace(bool fullScrenePlay);
			void EndDockSpace();

			void DrawMenuBar();

			//void DrawGrid2D();
			//void DrawGrid3D();


			// Editor Display

			void SetImGuizmoOperation(uint32_t operation)
			{
				m_imGuizmoOperation = operation;
			}
			uint32_t GetImGuizmoOperation() const
			{
				return m_imGuizmoOperation;
			}

			bool& ShowGizmos();
			bool& ShowViewSelected();

			void ToggleSnap();

			bool& FullScreenOnLaunch();

		
			bool& ShowGrid() { return m_properties.m_showGrid; }

			void SelectEntity(entt::entity entity);
			void UnselectEntity(entt::entity entity);
			void ClearSelectedEntity();
			const std::vector<entt::entity>& GetSelectedEntities() const;

			bool IsEntitySelected(entt::entity entity);


			bool IsCopied(entt::entity entity);
			void SetCopiedEntity(entt::entity entity, bool cut = false);
			const std::vector<entt::entity>& GetCopiedEntity()const;

			bool GetCutCopyEntity();



			void FocusCamera(const Vector3& targetpoint, float distance, float speed = 1.0f);

			void ShowPreview();
			void DrawPreview();

			// Raycast select
			void SelectObjectRay(const Ray& ray);


			EditorWindow* GetTextEditWindow();
			void RemoveEditorWindow(EditorWindow* window);

			FileBrowserWindow& GetFileBrowser() { return m_fileBrowserWindow; }

			Ray GetScreenRay(uint32_t xPos, uint32_t yPos, Camera* camera, uint32_t width, uint32_t height);

			void ActivateSceneView(bool active);
			void SetSceneViewSize(uint32_t width, uint32_t height);

			EditorState GetEditorState() { return  m_editorState; }
			void SetSceneActive(bool active) { m_sceneActive = active; }
			bool GetSceneActive() { return m_sceneActive; }

			Camera* GetCamera() const { return m_editorCamera.get(); }
			Transform& GetEditorCameraTransform() { return m_editorCameraTransform; }

			std::unordered_map<size_t, const char*>& GetComponentIconMap();

			struct EditorProperties
			{
				float m_gridSize = 100.0f;

				bool m_shouldDebugDraw = false;

				bool m_showGrid = true;
				bool m_showGizmos = true;
				bool m_snapQuizmo = false;
				float m_snapAmount = 1.0f;
				float m_imGuizmoScale = 0.25f;

				bool m_showViewSelected = false;
				bool m_view2D = false;

				bool m_fullscreenLaunch = false;
				bool m_fullscreenScene = false;

				ImGuiUtils::Theme m_theme = ImGuiUtils::Theme::Black;
				
				bool m_freeAspect = true;
				float m_fixedAspect = 1.0f;
				bool m_halfRes = false;
				float m_aspectRatio = 16.0f / 9.0f;


				float m_cameraNear = 0.01f;
				float m_cameraFar = 1000.0f;
				float m_scameraSpeed = 200.0f;

			};

			EditorProperties& GetProperties() { return m_properties; }
			Vector2 m_sceneViewPosition;

		protected:


			bool OnWindowResize(WindowResizeEvent& event);

			Vector3 m_cameraDestination = Vector3{ 0.0f };
			Vector3 m_cameraOrigin = Vector3{ 0.0f };

			bool m_sceneViewActive = false;
			bool m_sceneActive = false;
			bool m_editorActive = false;

			bool m_camerainTransition = false;
			float m_cameraTransitionStartTime = 0.0f;
			float m_cameraTransitionSpeed = 0.0f;

			EditorProperties m_properties;

			std::vector<SharedPtr<EditorWindow>> m_editorWindows;

			FileBrowserWindow m_fileBrowserWindow;

			std::vector<entt::entity> m_selectedEntities;
			std::vector<entt::entity> m_copiedEntities;

			bool m_cutCopyEntity = false;

			Camera* m_currentCamera = nullptr;
			Transform m_editorCameraTransform;
			SharedPtr<Camera> m_editorCamera = nullptr;

			EditorCameraController m_editorCameraController;


			std::unordered_map<size_t, const char*> m_componentIconMap;

			EditorState m_editorState = EditorState::Preview;

			uint32_t m_imGuizmoOperation = 14463;


			SharedPtr<Texture> m_previewTexture;
			SharedPtr<Mesh> m_previewSphere;

			uint32_t m_sceneViewWidth = 0;
			uint32_t m_sceneViewHeight = 0;
			bool m_sceneViewSizeUpdated = false;

		};


	}
}

#endif // !EDITOR_H

