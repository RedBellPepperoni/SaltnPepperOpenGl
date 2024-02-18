#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "Engine/Core/Rendering/Renderer/Renderer.h"


namespace SaltnPepperEngine
{

	class ShaderLibrary;
	class MaterialLibrary;
	class Scene;


	namespace Rendering
	{
		class FrameBuffer;

		class RenderManager
		{

		private:
			// The class that actually draws stuff
			UniquePtr<Renderer> m_renderer;

			// The Resource Library that stores all the shaders
			SharedPtr<ShaderLibrary> m_ShaderLibrary;

			SharedPtr<MaterialLibrary> m_MaterialLibrary;



			Scene* m_currentScene;


		private:

			void LoadEngineShaders();





		public:

			void Init();


			void ProcessScene(Scene* scene);

			// Start of the render Pipeline
			void RenderFrame();
			// End of the pipeline
			void EndFrame();



			void AttachFrameBuffer(SharedPtr<FrameBuffer>& frameBuffer);
			void AttachFrameBufferNoClear(SharedPtr<FrameBuffer>& frameBuffer);

			void RenderToAttachedFrameBuffer(SharedPtr<Shader>& shader);
			void RenderToFrameBuffer(SharedPtr<FrameBuffer>& frameBuffer, SharedPtr<Shader>& shader);
			void RenderToFrameBufferNoClear(SharedPtr<FrameBuffer>& frameBuffer,SharedPtr<Shader>& shader);

			void RenderToTexture(SharedPtr<Texture>& texture,SharedPtr<Shader>& shader, Attachment attachment = Attachment::COLOR_ATTACH_0);
			void RenderToTextureNoClear(SharedPtr<Texture>& texture, SharedPtr<Shader>& shader, Attachment attachment = Attachment::COLOR_ATTACH_0);


			void CopyTexture(SharedPtr<Texture> inputTexture, SharedPtr<Texture> outputTexture);

			void SetViewPort(int x, int y, int width, int height);
			void ProcessImage(SharedPtr<Texture>& texture, int lod = 0);

			void SetWindowSize(Vector2Int size);

			// Particle Stuff
			

			UniquePtr<Renderer>& GetRenderer();
			SharedPtr<ShaderLibrary>& GetShaderLibrary();
			SharedPtr<MaterialLibrary>& GetMaterialLibrary();


			CameraElement m_editorCameraElement;

			int mainCameraIndex = -1;

		};

	}
}



#endif // !RENDERMANAGER_H
