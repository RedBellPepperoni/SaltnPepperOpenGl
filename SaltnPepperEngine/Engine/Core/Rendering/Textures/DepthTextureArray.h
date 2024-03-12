#ifndef DEPTHTEXTUREARRAY_H
#define DEPTHTEXTUREARRAY_H
#include "Engine/Core/Rendering/Textures/Texture.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class DepthTextureArray
		{
			DepthTextureArray();
			DepthTextureArray(const size_t width, const size_t height, const size_t count, const TextureFormat format = TextureFormat::DEPTH32F);
			~DepthTextureArray();

			void Load(size_t width, size_t height, size_t count, TextureFormat format = TextureFormat::DEPTH32F);
			void Resize(size_t newWidth, size_t newHeight, size_t newCount);
			
			void Bind() const;
			void Bind(int id) const;
			void UnBind() const;
			int GetBoundId() const;
			uint32_t GetHandle() const;

			int GetCount() const;

		private:

			void DeleteTexture();

			uint32_t m_textureId;
			std::string name = "depthArray_";
			size_t m_width = 0;
			size_t m_height = 0;
			size_t m_count = 0;

			mutable int m_activeId = 0;

			TextureFormat m_format = TextureFormat::DEPTH32F;

		};

	}
}


#endif // !DEPTHTEXTUREARRAY_H

