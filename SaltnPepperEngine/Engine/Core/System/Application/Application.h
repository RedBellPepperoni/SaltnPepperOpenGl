#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/System/Events/Event.h"
#include "Engine/Core/System/Events/WindowEvent.h"

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Time/Time.h"

namespace SaltnPepperEngine
{


	class ModelLibrary;
	class TextureLibrary;
	class CubeMapLibrary;
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



	class Application
	{

	protected:

		// The reference to the current application -> there should only be one instance
		static Application* m_currentApplication;

		// The Manager which controls all the Rendering of the Applciation
		UniquePtr<RenderManager> m_renderManager;

		UniquePtr<Window> m_window = nullptr;

		UniquePtr<PhysicsEngine> m_physicsSystem = nullptr;

		UniquePtr<RuntimeEditor> m_editor = nullptr;


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
		SharedPtr<Scene> m_currentScene;


		UniquePtr<ImGuiManager> m_imguiManager;

		// Default Scale
		float m_timeScale = 1.0f;

		float m_deltaTime = 0.0f;
		float m_totalElapsedTime = 0.0f;
		size_t m_Fps = 0;


		// Future case when there can be multiple cameras // 
					// "-1"  means no rendering cameras were found
		int m_mainCameraIndex = -1;


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



	};
}


#endif // !APPLICATION_H

