#include "DepthTextureArray.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/GLUtils.h"

#include "Engine/Utils/Loading/ImageLoader.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		DepthTextureArray::DepthTextureArray()
			: m_width(0)
			, m_height(0)
			, m_count(0)
		{
			LOG_ERROR("DepthTextureArray Constructed with no width or height");
		}

		DepthTextureArray::DepthTextureArray(const size_t width, const size_t height, const size_t count, const TextureFormat format)
			: m_width(width)
			, m_height(height)
			, m_count(count)
			, m_format(format)
		{

			Load(width,height,count ,format);
		}

		DepthTextureArray::~DepthTextureArray()
		{
			DeleteTexture();
		}

		void DepthTextureArray::Load(size_t width, size_t height, size_t count, TextureFormat format)
		{
			GLDEBUG(glGenTextures(1, &m_textureId));
			GLDEBUG(glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId));

			GLDEBUG(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, m_count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
			GLDEBUG(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

		}

		void DepthTextureArray::Resize(size_t newWidth, size_t newHeight, size_t newCount)
		{
			m_width = newWidth;
			m_textureId = newHeight;
			m_count = newCount;

			GLDEBUG(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, m_count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
			
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

			GLDEBUG(glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
			GLDEBUG(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));

		}

		void DepthTextureArray::Bind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId));
		}

		void DepthTextureArray::Bind(int id) const
		{
			m_activeId = id;
			Bind();
		}

		void DepthTextureArray::UnBind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(GL_TEXTURE_2D_ARRAY, 0));
		}

		int DepthTextureArray::GetBoundId() const
		{
			return m_activeId;
		}

		uint32_t DepthTextureArray::GetHandle() const
		{
			return m_textureId;
		}

		int DepthTextureArray::GetCount() const
		{
			return m_count;
		}

		void DepthTextureArray::DeleteTexture()
		{
			GLDEBUG(glDeleteTextures(1, &m_textureId));
		}
	}
}
