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
#include "Engine/Core/Rendering/Geometry/Model.h"


#include "Engine/Core/Physics/SoftBody/Cloth.h"
#include "Engine/Core/Physics/SoftBody/SoftBody.h"


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

			CHECKNULL(GetShaderLibrary()->LoadShader("OpaqueForward", FileSystem::GetShaderDir().string() + "Opaque_Forward_Vert.glsl", FileSystem::GetShaderDir().string() + "Opaque_Forward_Frag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("OpaqueShadowed", FileSystem::GetShaderDir().string() + "Opaque_Forward_Vert.glsl", FileSystem::GetShaderDir().string() + "Opaque_FShadow_Frag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("TransparentShader", FileSystem::GetShaderDir().string() + "transparentVert.glsl", FileSystem::GetShaderDir().string() + "transparentFrag.glsl"));
			CHECKNULL(GetShaderLibrary()->LoadShader("ScreenShaderOne", FileSystem::GetShaderDir().string() + "chromaticShaderVert.glsl", FileSystem::GetShaderDir().string() + "chromaticShaderFrag.glsl"));
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

			ComponentView modelView = scene->GetEntityManager()->GetComponentsOfType<ModelComponent>();

			// Looping through all the entities that have a mesh component
			for (Entity modelObject : modelView)
			{

				//if (!modelObject.TryGetComponent<ActiveComponent>()->active)
				if (!modelObject.IsActive())
				{
					//The mesh is not visible , so dont need to render it
					continue;
				}

				// Cache the model ref and trasnfrom for later use
				ModelComponent& modelComp = modelObject.GetComponent<ModelComponent>();
				Transform& transform = modelObject.GetComponent<Transform>();

				const std::vector<SharedPtr<Mesh>>& meshes = modelComp.m_handle->GetMeshes();

				for (SharedPtr<Mesh> mesh : meshes)
				{
					Matrix4& worldTransform = transform.GetMatrix();
					
					// Check for frustum Optimization later
					// Might need to add bound boxes to each mesh for this
				
					const SharedPtr<Material>& material = mesh->GetMaterial();
				    
					m_renderer->ProcessRenderElement(mesh, material, transform);

				}

				// Sending the mesh data for processing
				
			}

			ComponentView lightView = scene->GetEntityManager()->GetComponentsOfType<Light>();

			for (Entity lightObject : lightView)
			{
				

				Light& lightComponent = lightObject.GetComponent<Light>(); 
				Transform* transform = &lightObject.GetComponent<Transform>(); 
						
				if (lightObject.IsActive())
				{
					m_renderer->ProcessLightElement(lightComponent, *transform);
				}
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
			
			mainCameraIndex = Application::GetCurrent().GetMainCameraIndex();

			if (mainCameraIndex < 0)
			{
				LOG_WARN("No Rendering Cameras");
				return;
			}

			
			


			if (Application::GetCurrent().GetEditorActive())
			{
				AttachFrameBuffer(m_editorCameraElement.gBuffer);

				GLDEBUG(glEnable(GL_DEPTH_TEST));
				m_renderer->Clear(true);

				//// ===== Post Render Skybox Pass =================
				m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), m_editorCameraElement);

				//// ===== Object Pass for Opaque Elements ================ 
				m_renderer->ObjectPass(m_ShaderLibrary->GetResource("OpaqueForward"), m_editorCameraElement, m_renderer->GetPipeLine().opaqueElementList);

				//// ===== Object Pass for Transparent Elements ================ 
				m_renderer->ObjectPass(m_ShaderLibrary->GetResource("TransparentShader"), m_editorCameraElement, m_renderer->GetPipeLine().transparentElementList);
				
				//// ===== Object Pass for Transparent Elements ================ 
				m_renderer->ObjectPass(m_ShaderLibrary->GetResource("ScreenShaderOne"), m_editorCameraElement, m_renderer->GetPipeLine().customElementList);


				m_editorCameraElement.depthTexture->GenerateMipMaps();

				m_renderer->DebugPass(m_editorCameraElement);

				m_renderer->AttachDefaultFrameBuffer();
			}

			else
			{

				for (const CameraElement& cameraElement : m_renderer->GetPipeLine().cameraList)
				{

					

					AttachFrameBuffer(cameraElement.gBuffer);

					GLDEBUG(glEnable(GL_DEPTH_TEST));
					m_renderer->Clear(true);

					// ===== Post Render Skybox Pass =================
					m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), cameraElement);


					// ===== Forward Pass for Opaque Elements ================ 
					m_renderer->ObjectPass(m_ShaderLibrary->GetResource("OpaqueShadowed"), cameraElement, m_renderer->GetPipeLine().opaqueElementList);

					//// ===== Object Pass for Transparent Elements ================ 
					m_renderer->ObjectPass(m_ShaderLibrary->GetResource("TransparentShader"), cameraElement, m_renderer->GetPipeLine().transparentElementList);

					//// ===== Object Pass for Transparent Elements ================ 
					m_renderer->ObjectPass(m_ShaderLibrary->GetResource("ScreenShaderOne"), cameraElement, m_renderer->GetPipeLine().customElementList);

					// Generate Depth mipmaps
					cameraElement.depthTexture->GenerateMipMaps();

					//ProcessImage()

					//CopyTexture(cameraElement.albedoTexture,cameraElement.outputTexture);
				
				}

				EndFrame();

			}
			
			
			


	

		}

		void RenderManager::EndFrame()
		{
			m_renderer->AttachDefaultFrameBuffer();

			if (mainCameraIndex < m_renderer->GetPipeLine().cameraList.size()) {

				CameraElement& mainCamera = m_renderer->GetPipeLine().cameraList[mainCameraIndex];

				if (!mainCamera.outputTexture)
				{
					LOG_ERROR("Invalid output camera texture");
				}


				glDisable(GL_DEPTH_TEST);
				m_renderer->Clear();
				ProcessImage(mainCamera.outputTexture);

			
			}
			
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

		void RenderManager::CopyTexture(SharedPtr<Texture> inputTexture, SharedPtr<Texture> outputTexture)
		{
			m_renderer->GetPostProcessFrameBuffer()->AttachTexture(outputTexture);
			AttachFrameBuffer(m_renderer->GetPostProcessFrameBuffer());
			ProcessImage(inputTexture);
		}

		void RenderManager::SetViewPort(int x, int y, int width, int height)
		{
			m_renderer->SetViewport(x, y, width, height);
		}

		void RenderManager::ProcessImage(const SharedPtr<Texture>& texture, int lod)
		{
			SharedPtr<Shader> screenRectShader = m_ShaderLibrary->GetResource("ScreenShader");
			m_renderer->RenderScreenQuad(screenRectShader,texture);
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