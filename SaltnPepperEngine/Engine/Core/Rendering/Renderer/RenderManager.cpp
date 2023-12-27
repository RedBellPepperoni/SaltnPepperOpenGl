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

			//CHECKNULL(GetShaderLibrary()->LoadShader("StandardShader", File::GetShaderDir().string() + "textureVert.glsl", File::GetShaderDir().string() + "textureFrag.glsl"));
			//CHECKNULL(GetShaderLibrary()->LoadShader("StandardShader", File::GetShaderDir().string() + "vert.glsl", File::GetShaderDir().string() + "frag.glsl"));

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

			//// Loop through all the cameras in the scene and send therr data to the pipeline 
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

				MeshComponent& meshComp = meshObject.GetComponent<MeshComponent>();

				if (!meshComp.isVisible)
				{
					//The mesh is not visible , so dont need to render it
					continue;
				}

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
			//GLDEBUG(glEnable(GL_CULL_FACE));
			//GLDEBUG(glCullFace(GL_BACK));

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

			// Store the data for the current rendering camera
			const CameraElement& cameraElement = m_renderer->GetPipeLine().cameraList[cameraIndex];

			// ===== Post Render Skybox Pass =================
			m_renderer->SkyBoxPass(m_ShaderLibrary->GetResource("SkyboxShader"), cameraElement);

			// ===== Forward Pass for Opaque Elements ================ 
			m_renderer->ForwardPass(m_ShaderLibrary->GetResource("StandardShader"), cameraElement, MaterialType::Opaque);


			m_renderer->DebugPass(cameraElement);





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