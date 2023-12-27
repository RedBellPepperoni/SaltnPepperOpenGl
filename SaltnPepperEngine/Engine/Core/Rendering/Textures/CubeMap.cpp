#include "CubeMap.h"
#include "Engine/Utils/GLUtils.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Core/Rendering/Textures/Image.h"
#include "Engine/Utils/Loading/ImageLoader.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
		void CubeMap::FreeCubeMap()
		{
			if (m_id != 0)
			{
				GLDEBUG(glDeleteTextures(1, &m_id));
				LOG_TRACE("CubeMap : Deleted cubemap with id = {0}", m_id);
			}

			m_id = 0;
			m_activeId = 0;
		}


		CubeMap::CubeMap()
		{
			GLDEBUG(glGenTextures(1, &m_id));
			LOG_TRACE("CubeMap : Created cubemap with id = {0}", m_id);
		}


		CubeMap::~CubeMap()
		{
			FreeCubeMap();
		}


		CubeMap::CubeMap(const FilePath& path)
			: CubeMap()
		{
			Load(path);
		}

		void CubeMap::Bind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));
		}

		void CubeMap::Bind(CubeMapBindId id) const
		{
			m_activeId = id;
			Bind();
		}


		void CubeMap::UnBind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		}

		CubeMap::CubeMapBindId CubeMap::getBoundId() const
		{
			return m_activeId;
		}


		CubeMap::BindableId CubeMap::GetHandle() const
		{
			return m_id;
		}

		void CubeMap::Load(const FilePath& filePath)
		{
			// Make this later on for simpler loading
		}

		void CubeMap::Load(const FilePath& right, const FilePath& left, const FilePath& top, const FilePath& bottom, const FilePath& front, const FilePath& back)
		{
			bool flipImage = false;
			std::array<Image, 6> images =
			{
				ImageLoader::LoadImageFromFile(right, flipImage),
				ImageLoader::LoadImageFromFile(left, flipImage),
				ImageLoader::LoadImageFromFile(top, flipImage),
				ImageLoader::LoadImageFromFile(bottom, flipImage),
				ImageLoader::LoadImageFromFile(back, flipImage),
				ImageLoader::LoadImageFromFile(front, flipImage),
			};

			Load(images);
		}

		void CubeMap::Load(const std::array<Image, 6>& images)
		{
			m_width = images.front().GetWidth();
			m_height = images.front().GetHeight();
			m_channels = images.front().GetChannelCount();

			GLenum pixelType = images.front().IsFloatingPoint() ? GL_FLOAT : GL_UNSIGNED_BYTE;
			GLenum pixelFormat = GL_RGBA;

			switch (m_channels)
			{
			case 1:
				pixelFormat = GL_RED;
				break;
			case 2:
				pixelFormat = GL_RG;
				break;
			case 3:
				pixelFormat = GL_RGB;
				break;
			case 4:
				pixelFormat = GL_RGBA;
				break;
			default:
				LOG_ERROR("CubeMap : invalid channel count: {0}", m_channels);
				break;
			}

			GLDEBUG(glBindTexture(GL_TEXTURE_CUBE_MAP, m_id));

			for (size_t index = 0; index < images.size(); index++)
			{
				GLDEBUG(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (GLenum)index, 0, GL_RGB,
					(GLsizei)m_width, (GLsizei)m_height, 0, pixelFormat, pixelType, images[index].GetRawData()));

			}


			GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

			if (std::any_of(images.begin(), images.end(), [](const Image& slice) {return slice.GetRawData() != nullptr; }))
			{
				GenerateMipMaps();
			}
			else
			{
				GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			}
		}


		void CubeMap::LoadDepth(int width, int height)
		{
		}

		size_t CubeMap::GetHeight() const
		{
			return m_height;
		}
		size_t CubeMap::GetWidth() const
		{
			return m_width;
		}
		size_t CubeMap::GetChannels() const
		{
			return m_channels;
		}

		void CubeMap::SetMaxLOD(size_t lod) const
		{
			Bind(0);
			GLDEBUG(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, (float)lod));
		}
		void CubeMap::SetMinLOD(size_t lod) const
		{
			Bind(0);
			GLDEBUG(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, (float)lod));
		}
		void CubeMap::GenerateMipMaps()
		{
			Bind(0);
			GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLDEBUG(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLDEBUG(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		}

    }
}