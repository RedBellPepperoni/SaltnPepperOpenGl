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
			void RenderFrame();

			UniquePtr<Renderer>& GetRenderer();


			SharedPtr<ShaderLibrary>& GetShaderLibrary();


			SharedPtr<MaterialLibrary>& GetMaterialLibrary();




		};

	}
}



#endif // !RENDERMANAGER_H
