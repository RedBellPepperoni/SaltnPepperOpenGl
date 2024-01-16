#ifndef IMGUIRENDERER_H
#define IMGUIRENDERER_H
#include <stdint.h>

struct GLFWwindow;


namespace SaltnPepperEngine
{
	
	namespace Editor
	{
		
		class ImGuiRenderer
		{
		public:

			ImGuiRenderer(bool clearScreen);
			~ImGuiRenderer();

			void Init();
			void NewFrame();
			void Render();
			void OnResize(uint32_t width, uint32_t height);

			void SetWindowRef(GLFWwindow* windowRef);

		
		private:

			GLFWwindow* m_windowHandle = nullptr;
			bool m_clearScreen = false;
		
		};
	}
}


#endif // !IMGUIRENDERER_H


