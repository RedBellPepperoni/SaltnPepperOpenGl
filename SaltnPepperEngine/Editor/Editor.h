#ifndef EDITOR_H
#define EDITOR_H

#include "EditorWindow.h"
#include "ImGuiUtils.h"

#include "Engine/Core/System/Application/Application.h"

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

			void OnUpdate();
			void OnDebugDraw();

			void SetEditorActive(bool active);
			bool& ToggleEditor();

			void BeginDockSpace(bool fullScrenePlay);
			void EndDockSpace();

			void DrawMenuBar();

			//void DrawGrid2D();
			//void DrawGrid3D();


			// Editor Display
			bool& ShowGizmos();
			bool& ShowViewSelected();
			bool& FullScreenOnLaunch();

		
			void SelectEntity(entt::entity entity);
			void UnselectEntity(entt::entity entity);
			void ClearSelectedEntity();
			const std::vector<entt::entity>& GetSelectedEntities() const;

			bool IsEntitySelected(entt::entity entity);

			void FocusCamera(const Vector3& targetpoint, float distance, float speed = 1.0f);

			void ShowPreview();
			void DrawPreview();

			// Raycast select
			void SelectObjectRay(const Ray& ray);


			EditorWindow* GetTextEditWindow();
			void RemoveEditorWindow(EditorWindow* window);


			Ray GetScreenRay(uint32_t xPos, uint32_t yPos, Camera* camera, uint32_t width, uint32_t height);

			void ActivateSceneView(bool active);


			struct EditorProperties
			{
				float m_gridSize = 100.0f;

				bool m_shouldDebugDraw = false;

				bool m_showGrid = true;
				bool m_showGizmos = true;

				bool m_showViewSelected = false;
				bool m_view2D = false;

				bool m_fullscreenLaunch = false;
				bool m_fullscreenScene = false;

				ImGuiUtils::Theme m_theme = ImGuiUtils::Theme::Black;
				float m_aspectRatio = 16.0f / 9.0f;


				float m_cameraNear = 0.01f;
				float m_cameraFar = 1000.0f;
				float m_scameraSpeed = 200.0f;

			};


			Vector2 m_sceneViewPosition;

		protected:


			bool OnWindowResize(WindowResizeEvent& event);

			Vector3 m_cameraDestition = Vector3{ 0.0f };
			Vector3 m_cameraOrigin = Vector3{ 0.0f };

			bool m_sceneViewActive = false;
			bool m_editorActive = false;

			bool m_camerainTransition = false;
			float m_cameraTrasitionStartTime = 0.0f;
			float m_cameraTransitionSpeed = 0.0f;

			EditorProperties m_properties;

			std::vector<SharedPtr<EditorWindow>> m_editorWindows;

			std::vector<entt::entity> m_selectedEntities;

			Camera* m_currentCamera = nullptr;

			Transform m_editorCameraTransform;
			SharedPtr<Camera> m_editorCamera = nullptr;

			EditorState m_editorState = EditorState::Preview;




			SharedPtr<Texture> m_previewTexture;
			SharedPtr<Mesh> m_previewSphere;

		};


	}
}

#endif // !EDITOR_H

