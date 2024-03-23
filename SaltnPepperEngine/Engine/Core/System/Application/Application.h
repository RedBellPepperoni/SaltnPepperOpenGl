#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/System/Events/Event.h"
#include "Engine/Core/System/Events/WindowEvent.h"

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Time/Time.h"
#include "Engine/Core/Scene/SceneManager.h"

#include "Engine/Utils/Serialization/Serialization.h"

namespace SaltnPepperEngine
{


	class ModelLibrary;
	class TextureLibrary;
	class CubeMapLibrary;

	namespace Audio
	{
		class AudioManager;
	}
	using Audio::AudioManager;
	
	class AudioLibrary;
	class GameObjectRegistry;
	class Scene;
	

	namespace Rendering
	{
		class Window;
		class RenderManager;
		class Camera;
	}

	using Rendering::RenderManager;
	using Rendering::Window;
	using Rendering::Camera;

	namespace Editor
	{
		class RuntimeEditor;
		class ImGuiManager;
	}

	using namespace Editor;

	namespace Physics
	{
		class PhysicsEngine;
	}

	using Physics::PhysicsEngine;


	namespace Components
	{
		class Transform;
	}

	using Components::Transform;

	class Application
	{
	public:

		struct ProjectSettings
		{
			std::string m_projectRoot;
			std::string m_projectName;
			std::string m_engineResourcePath;

			bool fullscreen = true;
			bool vSync = false;
			std::string title;
			int projectVersion;

		};



	protected:

		// The reference to the current application -> there should only be one instance
		static Application* m_currentApplication;

		// The Manager which controls all the Rendering of the Applciation
		UniquePtr<RenderManager> m_renderManager;

		UniquePtr<Window> m_window = nullptr;

		UniquePtr<PhysicsEngine> m_physicsSystem = nullptr;

		UniquePtr<RuntimeEditor> m_editor = nullptr;

		UniquePtr<SceneManager> m_sceneManager = nullptr;

		UniquePtr<AudioManager> m_audioSystem = nullptr;

		bool m_isRunning = false;
		bool m_isPaused = false;

		// Right now it points to the class given by M Feeney, change this later
		//SharedPtr<ShaderLibrary> shaderLibrary;

		// The  containers for the loaded models 
		SharedPtr<ModelLibrary> m_modelLibrary;

		// The container for all teh loaded texures
		SharedPtr<TextureLibrary> m_textureLibrary;

		// The Container for all Loaded Cubemaps
		SharedPtr<CubeMapLibrary> m_cubeMapLibrary;

		// The container for all loaded sounds
		SharedPtr<AudioLibrary> m_audioLibrary;

		// Pointer to the currently loaded scene
		Scene* m_currentScene;

		ProjectSettings m_projectSettings;

		UniquePtr<ImGuiManager> m_imguiManager;

		// Default Scale
		float m_timeScale = 1.0f;

		float m_deltaTime = 0.0f;
		float m_totalElapsedTime = 0.0f;
		size_t m_Fps = 0;


		// Future case when there can be multiple cameras // 
					// "-1"  means no rendering cameras were found
		int m_mainCameraIndex = -1;

		bool m_isProjectLoaded = false;


	protected:

		// Render all the Objects in the scene
		void RenderObjects();

		// Teh Constructor
		Application();

		// Inherited class methods to do stuff
		virtual void OnInit() {};
		virtual void OnCreate() {};
		virtual void OnUpdate(float deltaTime) {};
		

		void UpdateDeltaTime(float& lastFrameEnd, float& lastSecondEnd, size_t& fps);

		bool m_editorVisible = false;

	public:

	


		// Destructor
		virtual ~Application();

		// Reference Getter for the application
		static Application& GetCurrent();

		// Referene getter for the current window
		Window& GetAppWindow();

		// Reference Getter for teh current scene
		Scene* GetCurrentScene() const;

		PhysicsEngine* GetPhysicsEngine() const;

		// The start up setup for the apllication
		void Initialize();

		// This methods initaitzes the runtime and starts the gameloop
		void Run();

		void OnImGui();

		// Deals with the Event queue to be processed by the glfw runtime
		void ProcessEvent(EventBase& event);

		// Reference Getters to the renderManager
		UniquePtr<RenderManager>& GetRenderManager();

		// Reference getter to teh Model Library
		SharedPtr<ModelLibrary>& GetModelLibrary();

		// Reference getter to the Textue Library
		SharedPtr<TextureLibrary>& GetTextureLibrary();

		SharedPtr<CubeMapLibrary>& GetCubeMapLibrary();

		AudioManager* GetAudioSystem();

		SharedPtr<AudioLibrary>& GetAudioLibrary();

		// Returns the GLFW current time from the current glfwwindow
		float GetGLFWTime();

		// Gets the Total Elaspded Time
		float GetTotalElapsed() const;

		// Sets the Total Elapsed Time
		void SetTotalElapsed(float time);

		// Gets the current delta time
		float GetDelta() const;

		// Get the current unscaled Delta// not affected by time scale
		float GetUnscaledDelta() const;
		
		// Gets the Current FPS
		size_t GetFPS() const;


		const int GetMainCameraIndex() const;
		void SetMainCameraIndex(const int newIndex);

		void SetCursorPosition(Vector2 position);
		void SetCursorHidden(bool hidden);

		void SetWindowTitle(const std::string& newTitle);

		void SetSkyIntensity(float newIntensity);
		const float GetSkyIntensity();

		void StartPhysics(bool shouldstart);
		const Vector2Int GetWindowSize();

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		// Editor Variables

		RuntimeEditor* GetEditor() { return m_editor.get(); }

		Camera* GetEditorCamera();
		Transform* GetEditorCameraTransform();

		ImGuiManager* GetImguiManager() { return m_imguiManager.get(); }

		const bool GetEditorActive() const;

		void AddDefaultScene();
		SceneManager* GetSceneManager() const { return m_sceneManager.get(); }
		// Can Setup Project Laungin
		virtual void OnDefaultProject();

		ProjectSettings& GetProjectSettings() { return m_projectSettings; }

		const bool GetProjectLoaded() const { return m_isProjectLoaded; }
		void SetProjectLoaded(const bool loaded) { m_isProjectLoaded = loaded; }

		void Quit();


		virtual void Serialise();
		virtual void Deserialise();

		template <typename Archive>
		void save(Archive& archive) const
		{
			
			std::string path;

			std::vector<std::string> paths = m_sceneManager->GetSceneFilePaths();
			archive(cereal::make_nvp("Scenes", paths));
			archive(cereal::make_nvp("SceneIndex", m_sceneManager->GetCurrentSceneIndex()));
		
		
		}

		template <typename Archive>
		void load(Archive& archive)
		{
			int sceneIndex = 0;

			std::string test;
			std::vector<std::string> sceneFilePaths;
			archive(cereal::make_nvp("Scenes", sceneFilePaths));

			for (auto& filePath : sceneFilePaths)
			{
				m_sceneManager->AddFileToLoadList(filePath);
			}

			if (sceneFilePaths.size() == sceneIndex)
			{
				AddDefaultScene();
			}
		
			archive(cereal::make_nvp("SceneIndex", sceneIndex));
			m_sceneManager->SwitchScene(sceneIndex);
		}


	};
}


#endif // !APPLICATION_H

