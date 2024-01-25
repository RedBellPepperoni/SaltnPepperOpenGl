#include "RenderManager.h"

#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/EntitySystem/EntityManager.h"

#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Macros.h"
#include "Engine/Core/System/Application/Application.h"

#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Components/SceneComponents.h"

#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Lights/Light.h"
#include "Engine/Core/Rendering/Textures/CubeMap.h"

#include "Engine/Core/Rendering/RenderDefinitions.h"

#include "Engine/Core/Rendering/Camera/Camera.h"
#include "Engine/Core/Rendering/Buffers/FrameBuffer.h"


namespace SaltnPepperEngine
{
	using Components::MeshComponent;
	using Components::MeshRenderer;

	namespace Rendering
	{
		void RenderManager::LoadEngineShaders()
		{
			// Loading the Default Shader 
			// Add other Defaultr Shaders below <----

			CHECKNULL(GetShaderLibrary()->LoadShader("StandardShader", FileSystem::GetShaderDir().string() + "spaceshipVert.glsl", FileSystem::GetShaderDir().string() + "spaceshipFrag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("SkyboxShader", FileSystem::GetShaderDir().string() + "skyboxVert.glsl", FileSystem::GetShaderDir().string() + "skyboxFrag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("DebugLineShader", FileSystem::GetShaderDir().string() + "DebugLineVert.glsl", FileSystem::GetShaderDir().string() + "DebugLineFrag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("DebugPointShader", FileSystem::GetShaderDir().string() + "DebugPointVert.glsl", FileSystem::GetShaderDir().string() + "DebugPointFrag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("ScreenShader", FileSystem::GetShaderDir().string() + "renderBufferVert.glsl", FileSystem::GetShaderDir().string() + "renderBufferFrag.glsl"));
			
		}

		void RenderManager::ProcessScene(Scene* scene)
		{
			// Clear the Data of the Pipeline
			m_renderer->ClearRenderCache();

			// get all the Entities that have the "Camera" Component
			ComponentView cameraView = scene->GetEntityManager()->GetComponentsOfType<Camera>();


			// No rendering cameras found since the view returned empty
			if (cameraView.IsEmpty())
			{
				LOG_WARN("No cameras in the scene");
				return;
			}

			Camera* editorCameraRef = Application::GetCurrent().GetEditorCamera();
			Transform* editorCameraTransfrom = Application::GetCurrent().GetEditorCameraTransform();

			// Get the Editor camera as an Camera Elemnt;
			m_editorCameraElement = m_renderer->GenerateCameraElement(*editorCameraRef, *editorCameraTransfrom);

			// Loop through all the cameras in the scene and send therr data to the pipeline 
			for (Entity cameraObject : cameraView)
			{
				Camera& camera = cameraObject.GetComponent<Camera>();
				Transform& transform = cameraObject.GetComponent<Transform>();

				m_renderer->SetUpCameraElement(camera, transform);
			}



			// Getting a view of all the Objects which have a "mesh" componenet
			ComponentView meshView = scene->GetEntityManager()->GetComponentsOfType<MeshComponent>();

			// Looping through all the entities that have a mesh component
			for (Entity meshObject : meshView)
			{

				if (!meshObject.TryGetComponent<ActiveComponent>()->active)
				{
					//The mesh is not visible , so dont need to render it
					continue;
				}

				MeshComponent& meshComp = meshObject.GetComponent<MeshComponent>();

				// getting the required components
				MeshRenderer& materialComp = meshObject.GetComponent<MeshRenderer>();
				Transform& transform = meshObject.GetComponent<Transform>();


				
				// Sending the mesh data for processing
				 m_renderer->ProcessRenderElement(meshComp.GetMesh(), materialComp.GetMaterial(), transform);

			}

			ComponentView lightView = scene->GetEntityManager()->GetComponentsOfType<Light>();

			for (Entity lightObject : lightView)
			{
				Light& lightComponent = lightObject.GetComponent<Light>(); 
				Transform* transform = &lightObject.GetComponent<Transform>(); 
				m_renderer->ProcessLightElement(lightComponent, *transform);
			}



		}




		void RenderManager::Init()
		{


			// Initializing Shader Library to store all the Loaded Shaders
			m_ShaderLibrary = MakeShared<ShaderLibrary>();

			m_MaterialLibrary = MakeShared<MaterialLibrary>();

			// Loading all the shaders that are default to the engine
			LoadEngineShaders();


			GLDEBUG(glEnable(GL_DEPTH_TEST));
			GLDEBUG(glEnable(GL_STENCIL_TEST));

			// Initializing Defaults that will be reused / instanced
			m_renderer = MakeUnique<Renderer>();

			m_renderer->Init();

			SharedPtr<Shader> lineShader = m_ShaderLibrary->GetResource("DebugLineShader");
			SharedPtr<Shader> pointShader = m_ShaderLibrary->GetResource("DebugPointShader");

			m_renderer->SetupDebugShaders(lineShader, pointShader);

			//m_renderer->SetSkyboxCubeMap(m_Texture->GetResource("DefaultSkyBox");

		}

		void RenderManager::RenderFrame()
		{
			
			int cameraIndex = Application::GetCurrent().GetMainCameraIndex();

			if (cameraIndex < 0)
			{
				LOG_WARN("No Rendering Cameras");
				return;
			}

			//m_renderer->AttachDefaultFrameBuffer();

			// Second Render Pass for Editor
			 //Make a bool here to not render scene view when the editor is turned off


			Camera* editorCameraRef = Application::GetCurrent().GetEditorCamera();
	
			SharedPtr<FrameBuffer>& buffer = m_renderer->SecondaryFrameBuffer;
			SharedPtr<Texture>& texture = m_renderer->SecondaryTexture;
			AttachFrameBuffer(buffer);
			//buffer->AttachTexture(texture);
			

			buffer->Validate();


			m_renderer->Clear();

			

			//// ===== Post Render Skybox Pass =================
			//m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), m_editorCameraElement);

			//// ===== Forward Pass for Opaque Elements ================ 
			//m_renderer->ForwardPass(m_ShaderLibrary->GetResource("StandardShader"), m_editorCameraElement, MaterialType::Opaque);

			//RenderToTextureNoClear(editorCameraRef->GetRenderTexture(), m_ShaderLibrary->GetResource("StandardShader"));
			//RenderToTexture(editorCameraRef->GetRenderTexture(), m_ShaderLibrary->GetResource("StandardShader"));


			//	
			//AttachFrameBuffer(m_editorCameraElement.gBuffer);
			//m_editorCameraElement.gBuffer->AttachTexture(m_editorCameraElement.outputTexture);

			//// ===== Post Render Skybox Pass =================
			//m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), m_editorCameraElement);

			//// ===== Forward Pass for Opaque Elements ================ 
			//m_renderer->ForwardPass(m_ShaderLibrary->GetResource("StandardShader"), m_editorCameraElement, MaterialType::Opaque);

			//RenderToTextureNoClear(m_editorCameraElement.outputTexture, m_ShaderLibrary->GetResource("StandardShader"));
			//RenderToTexture(m_editorCameraElement.outputTexture, m_ShaderLibrary->GetResource("StandardShader"));

			//
			//

			// Multiple Camera Rendering for actual Game View
			for (const CameraElement& cameraElement : m_renderer->GetPipeLine().cameraList)
			{
				//if (cameraElement.shouldRenderToTexture) { continue; }

				// Store the data for the current rendering camera
				//const CameraElement& cameraElement = m_renderer->GetPipeLine().cameraList[cameraIndex];

				/*AttachFrameBuffer(cameraElement.gBuffer);
				cameraElement.gBuffer->AttachTexture(cameraElement.outputTexture);*/

				// ===== Post Render Skybox Pass =================
				m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), cameraElement);

				// ===== Forward Pass for Opaque Elements ================ 
				m_renderer->ForwardPass(m_ShaderLibrary->GetResource("StandardShader"), cameraElement, MaterialType::Opaque);

				m_renderer->DebugPass(cameraElement);
			}

			//const CameraElement & cameraElement = m_renderer->GetPipeLine().cameraList[cameraIndex];

			m_renderer->AttachDefaultFrameBuffer();

			if (!texture)
			{
				LOG_ERROR("Invalid output camera texture");
			}

			GLDEBUG(glDisable(GL_DEPTH_TEST));
		   // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_renderer->RenderScreenQuad(m_ShaderLibrary->GetResource("ScreenShader"), texture);

			m_renderer->ClearRectangleObjectVAO();

			//EndFrame();

		}

		void RenderManager::EndFrame()
		{
			m_renderer->AttachDefaultFrameBuffer();

			if (!m_editorCameraElement.outputTexture)
			{
				LOG_ERROR("Invalid output camera texture");
			}

			m_renderer->RenderScreenQuad(m_ShaderLibrary->GetResource("ScreenShader"), m_editorCameraElement.outputTexture);

			m_renderer->ClearRectangleObjectVAO();
		}

		void RenderManager::AttachFrameBuffer(const SharedPtr<FrameBuffer>& frameBuffer)
		{
			AttachFrameBufferNoClear(frameBuffer);

			frameBuffer->Validate(); 

			GetRenderer()->Clear();
		}

		void RenderManager::AttachFrameBufferNoClear(const SharedPtr<FrameBuffer>& frameBuffer)
		{
			frameBuffer->Bind();

			int width = (int)frameBuffer->GetWidth();
			int height = (int)frameBuffer->GetHeight();

			SetViewPort(0, 0, width, height);
			
		}

		

		void RenderManager::RenderToAttachedFrameBuffer(const SharedPtr<Shader>& shader)
		{
			m_renderer->RenderToAttachedFrameBuffer(shader);	
		}

		void RenderManager::RenderToFrameBuffer(const SharedPtr<FrameBuffer>& frameBuffer, const SharedPtr<Shader>& shader)
		{
			AttachFrameBuffer(frameBuffer);
			RenderToAttachedFrameBuffer(shader);
		}

		void RenderManager::RenderToFrameBufferNoClear(const SharedPtr<FrameBuffer>& frameBuffer, const SharedPtr<Shader>& shader)
		{
			AttachFrameBufferNoClear(frameBuffer);
			RenderToAttachedFrameBuffer(shader);
		}

		void RenderManager::RenderToTexture(const SharedPtr<Texture>& texture, const SharedPtr<Shader>& shader, Attachment attachment)
		{
			//Bind Texture to use for the frameBuffer
			m_renderer->GetPostProcessFrameBuffer()->AttachTexture(texture, attachment);
			RenderToFrameBuffer(m_renderer->GetPostProcessFrameBuffer(), shader);
		}

		void RenderManager::RenderToTextureNoClear(const SharedPtr<Texture>& texture, const SharedPtr<Shader>& shader, Attachment attachment)
		{
			m_renderer->GetPostProcessFrameBuffer()->AttachTexture(texture, attachment);
			RenderToFrameBufferNoClear(m_renderer->GetPostProcessFrameBuffer(), shader);
		}

		void RenderManager::SetViewPort(int x, int y, int width, int height)
		{
			m_renderer->SetViewport(x, y, width, height);
		}

		void RenderManager::ProcessImage(const SharedPtr<Texture>& texture, int lod)
		{
			SharedPtr<Shader> screenRectShader = m_ShaderLibrary->GetResource("ScreenShader");

		}

		void RenderManager::SetWindowSize(Vector2Int size)
		{
			m_renderer->SetWindowSize(size);
		}

		UniquePtr<Renderer>& RenderManager::GetRenderer()
		{
			return m_renderer;
		}



		SharedPtr<ShaderLibrary>& RenderManager::GetShaderLibrary()
		{
			return m_ShaderLibrary;
		}
		SharedPtr<MaterialLibrary>& RenderManager::GetMaterialLibrary()
		{
			return m_MaterialLibrary;
		}


	}
}