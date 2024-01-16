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

#include "Engine/Core/Physics/PhysicsEngine/PhysicsEngine.h"
#include "Editor/Editor.h"
#include "Editor/ImGuiManager.h"


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

		// Creating and Initializing the Open GL Window
		m_window = MakeUnique<Window>();

		m_window->Initialize();

		// Setting up teh Model and Object Library
		m_modelLibrary = MakeShared<ModelLibrary>();
		m_textureLibrary = MakeShared<TextureLibrary>();
		m_cubeMapLibrary = MakeShared<CubeMapLibrary>();
		//m_audioLibrary = MakeShared<AudioLibrary>();


		

		m_currentScene = MakeShared<Scene>("testScene");

		m_physicsSystem = MakeUnique<PhysicsEngine>();

		// Initializing teh Render manager
		m_renderManager = MakeUnique<RenderManager>();


		m_editor = MakeUnique<RuntimeEditor>();


		// Setting the Instance reference of the creatd application
		m_currentApplication = this;

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
		return m_currentScene.get();
	}

	PhysicsEngine* Application::GetPhysicsEngine() const
	{
		return m_physicsSystem.get();
	}

	void Application::Initialize()
	{



		this->m_window->SetEventCallback(BIND_FN(Application::ProcessEvent));

		// Right now only limited to one scene -> change this to a load scene function later
		m_currentScene->Init();

		m_mainCameraIndex = 0;

		m_physicsSystem->Init();
		m_physicsSystem->UpdateScene(m_currentScene.get());

		m_physicsSystem->SetPaused(false);

		m_renderManager->Init();

		m_editor->OnInit();
		ImGui::CreateContext();	
		ImGui::StyleColorsDark();
		

		m_imguiManager = MakeUnique<ImGuiManager>(false);
		m_imguiManager->Init();
		


		m_editor->ToggleEditor();

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

			if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Escape))
			{
				m_window->CloseWindow();
				m_isRunning = false;
				break;
			}


			UpdateDeltaTime(frameEnd, secondEnd, frames);

			m_currentScene->Update(m_deltaTime);


			Input::InputSystem::GetInstance().ResetKeyPressed();



			m_window->PollEvents();


			// Update window and listen and process window events
			m_window->UpdateViewPort();
			m_window->UpdateImGui();

			ImGui::NewFrame();

			m_imguiManager->Update(m_deltaTime, GetCurrentScene());
		

			// Render all the vertices in the current Render array
			RenderObjects();

			m_window->RenderImGui();
			m_window->SwapBuffers();


			m_physicsSystem->Update(m_deltaTime);

			m_physicsSystem->UpdateECSTransforms();
//
			

			m_imguiManager->OnRender(GetCurrentScene());

			OnUpdate(m_deltaTime);


			if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::GraveAccent))
			{
				m_editorVisible = !m_editorVisible;
				//m_editor->Toggle(m_editorVisible);
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
		m_physicsSystem->SetPaused(shouldstart);


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




}
