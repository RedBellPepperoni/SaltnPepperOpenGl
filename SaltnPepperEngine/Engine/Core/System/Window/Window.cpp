#include "Window.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Engine/Utils/GLUtils.h"
#include "Editor/ImGuiUtils.h"

#include "Engine/Core/System/Events/WindowEvent.h"
#include "Engine/Core/System/Events/KeyboardEvent.h"
#include "Engine/Core/System/Events/MouseEvent.h"
#include "Engine/Core/System/Input/InputKeyCodes.h"

#include "Engine/Utils/Logging/Log.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"

#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"


namespace SaltnPepperEngine
{

	namespace Rendering
	{
        static void error_callback(int error, const char* description)
        {
            fprintf(stderr, "Error: %s\n", description);
        }



        inline void Window::WindowSizeCallback(GLFWwindow* window, int newWidth, int newHeight)
        {
            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            WindowResizeEvent resizeevent(Vector2Int(handle.m_properties.width, handle.m_properties.height), Vector2Int(newWidth, newHeight));
            handle.CallEvent(resizeevent);

        }

        inline void Window::WindowCloseCallback(GLFWwindow* window)
        {

            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));
            WindowCloseEvent event;

            handle.CallEvent(event);

        }

        inline void Window::WindowFocusCallback(GLFWwindow* window, int focus)
        {
            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            if (focus)
            {
                WindowFocusEvent event;
                handle.CallEvent(event);
            }
            else
            {
                WindowLostFocusEvent event;
                handle.CallEvent(event);
            }
        }

        inline void Window::WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {


            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyDownEvent pressedevent(Input::OpenGLKeys::GetKeyboardKey(key), 0);
                handle.CallEvent(pressedevent);
                break;
            }

            case GLFW_RELEASE:
            {
                KeyUpEvent releasedevent(Input::OpenGLKeys::GetKeyboardKey(key));
                handle.CallEvent(releasedevent);
                break;
            }

            case GLFW_REPEAT:
            {
                KeyDownEvent repeatevent(Input::OpenGLKeys::GetKeyboardKey(key), 1);
                handle.CallEvent(repeatevent);
                break;
            }
            }

        }

        inline void Window::WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            switch (action)
            {

            case GLFW_PRESS:
            {
                MouseButtonDownEvent clickevent(Input::OpenGLKeys::GetOpenGLMouseButton(button));
                handle.CallEvent(clickevent);

                break;
            }

            case GLFW_RELEASE:
            {
                MouseButtonUpEvent releaseevent(Input::OpenGLKeys::GetOpenGLMouseButton(button));
                handle.CallEvent(releaseevent);

                break;

            }

            default:
                break;
            }

        }

        inline void Window::WindowMouseScrollCallback(GLFWwindow* window, double xDelta, double yDelta)
        {
            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            MouseScrolledEvent scrollevent((float)xDelta, (float)yDelta);

            handle.CallEvent(scrollevent);
        }

        inline void Window::WindowCursorCallback(GLFWwindow* window, double xPos, double yPos)
        {
            Window& handle = *static_cast<Window*>((glfwGetWindowUserPointer(window)));

            MouseMovedEvent moveevent((float)xPos, (float)yPos);

            handle.CallEvent(moveevent);

        }








        Window::Window()
        {
            m_properties.width = 1920;
            m_properties.height = 1080;
            m_properties.title = "OpenGl Window!!";



        }

        Window::Window(const WindowProperties& properties)

        {
            m_properties.width = properties.width;
            m_properties.height = properties.height;
            m_properties.title = properties.title;



        }

        Window::~Window()
        {
        }


        void Window::UpdateViewPort()
        {

            glfwGetFramebufferSize(windowHandle, &m_properties.width, &m_properties.height);

            m_properties.aspectRatio = m_properties.width / (float)m_properties.height;

            GLDEBUG(glViewport(0, 0, m_properties.width, m_properties.height));
            GLDEBUG(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
            // GLDEBUG(glClearColor(0.7f, 0.7f, 0.7f, 1.0f));


            // Might Need to change This Later
            GLDEBUG(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


        }

        void Window::PollEvents()
        {

            glfwPollEvents();
        }

        void Window::SetEventCallback(const EventCallBackFn& callback)
        {
            m_properties.eventHandler = callback;
        }

        void Window::CallEvent(EventBase& event)
        {
            if (!m_properties.eventHandler)
            {
                return;
            }

            m_properties.eventHandler(event);
        }

        void Window::Initialize()
        {



            const unsigned int NumOfVertices = 3;


            glfwSetErrorCallback(error_callback);

            if (!glfwInit())
                exit(EXIT_FAILURE);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

            windowHandle = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), NULL, NULL);



            if (!windowHandle)
            {
                glfwTerminate();

                // std::cout << "MegaIssue";

                exit(EXIT_FAILURE);

                // isWindowOpen = false;
            }


            glfwSetInputMode(windowHandle, GLFW_STICKY_KEYS, true);

            glfwSetWindowUserPointer(windowHandle, this);



            glfwMakeContextCurrent(windowHandle);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            //glfwSetWindowSizeCallback(windowHandle, WindowSizeCallback);

            //glfwSetWindowCloseCallback(windowHandle, WindowCloseCallback);

           // glfwSetWindowFocusCallback(windowHandle, WindowFocusCallback);

            glfwSetKeyCallback(windowHandle, WindowKeyCallback);

            glfwSetMouseButtonCallback(windowHandle, WindowMouseButtonCallback);

            glfwSetScrollCallback(windowHandle, WindowMouseScrollCallback);

            glfwSetCursorPosCallback(windowHandle, WindowCursorCallback);


            SetVsync(m_properties.hasVSync);

            InitializeImGui();


        }

        void Window::InitializeImGui()
        {
           /* ImGuiContext* context = ImGui::CreateContext();
            ImGui::SetCurrentContext(context);

            ImGuiIO& imguiIO = ImGui::GetIO();

            imguiIO.ConfigFlags |= ImGuiConfigFlags_::ImGuiConfigFlags_DockingEnable;
            imguiIO.ConfigDockingAlwaysTabBar = true;

            ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);

            ImGui_ImplOpenGL3_Init();

            ImGui::StyleColorsDark();*/


            //ImGui::PushFont(ImGui::GetFont());

        }

        void Window::UpdateImGui()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();



        }

        void Window::RenderImGui()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        }

        bool Window::isOpen()
        {
            if (windowHandle == nullptr)
            {
                // Log error here that the pointer to the wwindow doesnt exists    
               // printf("Error creating window");

                return false;
            }

            return !glfwWindowShouldClose(windowHandle);

        }

        void Window::SwapBuffers()
        {
            glfwSwapBuffers(windowHandle);

        }

        void Window::CloseWindow()

        {
            glfwDestroyWindow(windowHandle);

            glfwTerminate();
            exit(EXIT_SUCCESS);
        }

        float Window::GetGLFWTime() const
        {
            return (float)glfwGetTime();
        }

        void Window::SetWindowTitle(const std::string& newTitle)
        {
            m_properties.title = newTitle;

            glfwSetWindowTitle(windowHandle, newTitle.c_str());
        }


        void Window::SetMousePosition(Vector2 position)
        {
            glfwSetCursorPos(windowHandle, position.x, position.y);
        }

        void Window::SetMouseHidden(bool isHidden)
        {
            if (isHidden)
            {
                glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        void Window::ToggleWireframe()
        {
            wireFrame = !wireFrame;

            if (wireFrame)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

        }


        const Vector2Int Window::GetSize() const
        {
            if (windowHandle == nullptr)
            {
                return Vector2Int(m_properties.width, m_properties.height);
            }

            int width, height;
            glfwGetWindowSize(windowHandle, &width, &height);

            return Vector2Int(width, height);
        }


        void Window::SetVsync(bool isVsync)
        {
            // Set the new vsync
            m_properties.hasVSync = isVsync;
            glfwSwapInterval(isVsync ? 1 : 0);

            LOG_INFO("Vsync");

        }


	}
}