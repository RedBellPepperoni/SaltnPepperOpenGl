  #include "Application.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/Time/Time.h"
#include "Engine/Core/System/Input/InputSystem.h"

#include "Engine/Core/System/Window/Window.h"

#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Macros.h"
#include "Engine/Core/Rendering/Renderer/RenderManager.h"
#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Components/Transform.h"

#include "Engine/Core/Physics/PhysicsEngine/PhysicsEngine.h"
#include "Editor/Editor.h"
#include "Editor/ImGuiManager.h"
#include "Engine/Core/EntitySystem/EntityManager.h"


namespace SaltnPepperEngine
{
	Application* Application::m_currentApplication = nullptr;

	Application::Application()
	{
		if (m_currentApplication != nullptr)
		{
			printf("Application Instance already exists");
			return;
		}

		// initializing the Logging
		Debug::Log::OnInit();

		// Setting the Instance reference of the creatd application
		m_currentApplication = this;

		// Creating and Initializing the Open GL Window
		m_window = MakeUnique<Window>();
		m_window->Initialize();

		// Initializing teh Render manager
		m_renderManager = MakeUnique<RenderManager>();

		// Setting up teh Model and Object Library
		m_modelLibrary = MakeShared<ModelLibrary>();
		m_textureLibrary = MakeShared<TextureLibrary>();
		m_cubeMapLibrary = MakeShared<CubeMapLibrary>();
		//m_audioLibrary = MakeShared<AudioLibrary>();


		m_physicsSystem = MakeUnique<PhysicsEngine>();
		m_physicsSystem->Init();

		m_sceneManager = MakeUnique<SceneManager>();
		m_sceneManager->EnqueueScene("New Scene");
		m_sceneManager->SwitchScene(0);
		m_sceneManager->ApplySceneSwitch();

		m_currentScene = m_sceneManager->GetCurrentScene();
		m_currentScene->Init();

		m_mainCameraIndex = 0;

		
	
		//m_physicsSystem->UpdateScene(m_currentScene);
		


		m_editor = MakeUnique<RuntimeEditor>();


		

		// Activate the User function
		OnCreate();



	}

	void Application::OnImGui()
	{
		m_editor->OnImGui();
	}

	void Application::UpdateDeltaTime(float& lastFrameEnd, float& lastSecondEnd, size_t& fps)
	{
		// Get the current time from the glfw runtime
		float currentTime = Time::CurrentEngineTime();

		// IF the engine is paused, don't update anything
		if (m_isPaused)
		{
			lastFrameEnd = 0.0f;
			lastSecondEnd = 0.0f;
			fps = 0;
			m_Fps = 0;
			m_deltaTime = 0;
		}

		// Otherwise calcaulte the delta time and the fps for the current - previous frame
		else
		{
			// Increase till we reach the frame end time
			fps++;


			if (lastFrameEnd - lastSecondEnd >= 1.0f)
			{
				// once we are at the end, reset the fps counter
				m_Fps = fps;
				lastSecondEnd = currentTime;
				fps = 0;

			}

			// Update the deltatime
			m_deltaTime = m_timeScale * (currentTime - lastFrameEnd);

			// Update the total time since start
			m_totalElapsedTime += m_deltaTime;
		}


		lastFrameEnd = currentTime;

	}





	void Application::RenderObjects()
	{
		// Tell the Render Manager to Do Drawing
		m_renderManager->ProcessScene(GetCurrentScene());
		m_renderManager->RenderFrame();

	}



	Application::~Application()
	{


	}


	Application& Application::GetCurrent()
	{
		return *m_currentApplication;
	}

	Window& Application::GetAppWindow()
	{
		return *m_window.get();
	}

	Scene* Application::GetCurrentScene() const
	{
		return m_sceneManager->GetCurrentScene();
	}

	PhysicsEngine* Application::GetPhysicsEngine() const
	{
		return m_physicsSystem.get();
	}

	void Application::Initialize()
	{



		m_window->SetEventCallback(BIND_FN(Application::ProcessEvent));

		

		//m_physicsSystem->SetPaused(false);
		//bool testPause = m_physicsSystem->GetIsPaused();

		m_renderManager->Init();


		
		m_editor->OnInit();

		
		m_imguiManager = MakeUnique<ImGuiManager>(false);
		m_imguiManager->Init();


		//m_editor->ToggleEditor();

		//LuaManager::GetInstance().OnInit();



		

		// Calling Init on the child applications
		OnInit();
	}

	void Application::Run()
	{
		// Set the engine as running
		m_isRunning = true;

		// container to store variables to calculate the delta timing
		float secondEnd = Time::CurrentEngineTime();
		float frameEnd = Time::CurrentEngineTime();
		size_t frames = 0;



		// Render Loop

		while (m_window->isOpen())
		{

			if (m_sceneManager->GetSwitchingScene())
			{
				m_sceneManager->ApplySceneSwitch();
				continue;
			}


			UpdateDeltaTime(frameEnd, secondEnd, frames);

			m_currentScene->Update(m_deltaTime);


			Input::InputSystem::GetInstance().ResetKeyPressed();



			m_window->PollEvents();


			// Update window and listen and process window events
			m_window->UpdateViewPort();
			m_renderManager->SetWindowSize(m_window->GetSize());

			//m_window->UpdateImGui();

			// Render all the vertices in the current Render array
			RenderObjects();
		
			if (InputSystem::GetInstance().GetKeyDown(Key::GraveAccent))
			{
				m_editor->ToggleEditor();
			}
			
			if (m_editor->IsEditorActive())
			{
				m_editor->OnUpdate(m_deltaTime);


				m_imguiManager->Update(m_deltaTime, GetCurrentScene());

				// Do all the ImGUI Rendering before Swapping the buffers
				m_imguiManager->OnRender(GetCurrentScene());

				m_physicsSystem->DebugDraw();
			}

			// Swapping the framebuffers 
			m_window->SwapBuffers();
			

			// Do the Physics Update here
			m_physicsSystem->Update(m_deltaTime);
			m_physicsSystem->UpdateECSTransforms(m_sceneManager->GetCurrentScene());
//
			

			OnUpdate(m_deltaTime);


			if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::GraveAccent))
			{
				m_editorVisible = !m_editorVisible;
				//m_editor->Toggle(m_editorVisible);
			}

			if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::G))
			{
				bool testPause = m_physicsSystem->GetIsPaused();
				m_physicsSystem->SetPaused(!testPause);
				
			}



		}

		m_window->CloseWindow();
	}

	void Application::ProcessEvent(EventBase& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<WindowCloseEvent>(BIND_FN(Application::OnWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_FN(Application::OnWindowResize));


		if (m_imguiManager)
		{
			m_imguiManager->OnEvent(event);
		}
		
		// Event already handled
		if (event.GetHandled()) { return; }

		Input::InputSystem::GetInstance().ProcessEvent(event);
	}

	UniquePtr<RenderManager>& Application::GetRenderManager()
	{
		return m_renderManager;
	}



	SharedPtr<ModelLibrary>& Application::GetModelLibrary()
	{
		return m_modelLibrary;
	}

	SharedPtr<TextureLibrary>& Application::GetTextureLibrary()
	{
		return m_textureLibrary;
	}

	SharedPtr<CubeMapLibrary>& Application::GetCubeMapLibrary()
	{
		return m_cubeMapLibrary;
	}

	SharedPtr<AudioLibrary>& Application::GetAudioLibrary()
	{
		return m_audioLibrary;
	}

	float Application::GetGLFWTime()
	{
		return m_window->GetGLFWTime();
	}

	float Application::GetTotalElapsed() const
	{
		return m_totalElapsedTime;
	}

	void Application::SetTotalElapsed(float time)
	{
		m_totalElapsedTime = time > 0.0f ? time : 0.0f;
	}

	float Application::GetDelta() const
	{
		return m_deltaTime;
	}

	float Application::GetUnscaledDelta() const
	{
		return m_deltaTime / m_timeScale;
	}

	size_t Application::GetFPS() const
	{
		return m_Fps;
	}

	const int Application::GetMainCameraIndex() const
	{
		return m_mainCameraIndex;
	}

	void Application::SetMainCameraIndex(const int newIndex)
	{
		size_t count = GetCurrentScene()->GetRegistry().view<Camera>().size();

		int index = newIndex;

		if (newIndex > count)
		{
			LOG_WARN("Out of bound index for camera");

			index = 0;
		}


		m_mainCameraIndex = index;

		LOG_ERROR("SetMainCameraIndex function Turned OFF");
	}

	void Application::SetCursorPosition(Vector2 position)
	{
		m_window->SetMousePosition(position);
	}

	void Application::SetCursorHidden(bool hidden)
	{
		m_window->SetMouseHidden(hidden);
	}

	void Application::SetWindowTitle(const std::string& newTitle)
	{


		GetAppWindow().SetWindowTitle(newTitle);
	}

	void Application::SetSkyIntensity(float newIntensity)
	{
		GetRenderManager()->GetRenderer()->SetSkyboxIntensity(newIntensity);
	}

	const float Application::GetSkyIntensity()
	{
		return GetRenderManager()->GetRenderer()->GetSkyboxIntensity();
	}

	void Application::StartPhysics(bool shouldstart)
	{
		m_physicsSystem->SetPaused(!shouldstart);

	}

	const Vector2Int Application::GetWindowSize()
	{
		return GetAppWindow().GetSize();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		return false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		return false;
	}

	Camera* Application::GetEditorCamera()
	{
	    return m_editor->GetCamera(); 
	}

	Transform* Application::GetEditorCameraTransform()
	{
		return &m_editor->GetEditorCameraTransform();
	}

	const bool Application::GetEditorActive() const
	{
		return m_editor->IsEditorActive();;
	}

	void Application::AddDefaultScene()
	{
		if (m_sceneManager->GetScenes().size() == 0)
		{
			m_sceneManager->EnqueueScene("Empty Scene");
			m_sceneManager->SwitchScene(0);
		}
	}

	void Application::OnDefaultProject()
	{
		m_projectSettings.fullscreen = true;
		m_projectSettings.m_projectName = "SerializationTest";
		m_projectSettings.m_engineResourcePath = FileSystem::GetEngineDir().u8string();
		//m_projectSettings.m_engineResourcePath = "..\\..\\TestProject\\";
	}

	void Application::Quit()
	{
		m_window->CloseWindow();
		m_isRunning = false;
		
	}

	void Application::Serialise()
	{
		/*{
			std::stringstream storage;
			{
				 output finishes flushing its contents when it goes out of scope
				cereal::JSONOutputArchive output{ storage };
				output(*this);
			}

			std::string path = FileSystem::GetCurrentPath().u8string();

			std::string fullPath =  path + std::string(".spproj");
			LOG_INFO("Serialising Application {0}", fullPath);
			FileSystem::WriteFileToText(fullPath, storage.str());
		}*/
	}

	void Application::Deserialise()
	{
	}




}
