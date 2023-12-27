#include "Texture.h"
#include "Engine/Utils/GLUtils.h"

#include "Engine/Utils/Loading/ImageLoader.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {

		GLenum GetGLFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::R:   return GL_R8;
				break;
			case TextureFormat::R16: return GL_R16;
				break;
			case TextureFormat::RG: return GL_RG8; 
				break;
			case TextureFormat::RG16: return GL_RG16; 
				break;
			case TextureFormat::R16F: return GL_R16F; 
				break;
			case TextureFormat::R32F: return GL_R32F; 
				break; 
			case TextureFormat::RG16F: return GL_RG16F; 
				break;
			case TextureFormat::RG32F: return GL_RG32F; 
				break;
			case TextureFormat::RGB: return GL_RGB; 
				break;
			case TextureFormat::RGBA: return GL_RGBA; 
				break;
			case TextureFormat::RGB16: return GL_RGB16; 
				break;
			case TextureFormat::RGB16F: return GL_RGB16F; 
				break;
			case TextureFormat::RGBA16: return GL_RGBA16; 
				break;
			case TextureFormat::RGBA16F:return GL_RGBA16F; 
				break;
			case TextureFormat::RGB32F:return GL_RGB32F; 
				break;
			case TextureFormat::RGBA32F:return GL_RGBA32F; 
				break;
			case TextureFormat::DEPTH:return GL_DEPTH_COMPONENT; 
				break;
			case TextureFormat::DEPTH32F:return GL_DEPTH_COMPONENT32F; 
				break;
			default:  return GL_RGB;
				break;
			}
		}


		GLint WrapTable[] =
		{
			 GL_CLAMP_TO_EDGE,
			GL_CLAMP_TO_BORDER,
			GL_MIRRORED_REPEAT,
			GL_REPEAT
		};


		void Texture::DeleteTexture()
		{
			if (m_textureId != 0)
			{
				GLDEBUG(glDeleteTextures(1, &m_textureId));
			}

			m_textureId = 0;
			m_activeId = 0;
		}


		Texture::Texture()
		{
			GLDEBUG(glGenTextures(1, &m_textureId));
			LOG_INFO("Texture : created with Id : {0}", m_textureId);
		}


		Texture::~Texture()
		{
			DeleteTexture();
		}

		void Texture::Bind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(m_textureType, m_textureId));

		}

		void Texture::Bind(int id) const
		{
			m_activeId = id;
			Bind();
		}

		void Texture::UnBind() const
		{
			GLDEBUG(glActiveTexture(GL_TEXTURE0 + m_activeId));
			GLDEBUG(glBindTexture(m_textureType, 0));
		}

		int Texture::GetBoundId() const
		{
			return m_activeId;
		}
		uint32_t Texture::GetHandle() const
		{
			return m_textureId;
		}


		Texture::Texture(const std::string& filePath, TextureFormat format)
			: Texture()
		{
			Load(filePath, format);
		}


		void Texture::Load(const FilePath& path, TextureFormat format)
		{
			bool flipImage = true;
			Image image = ImageLoader::LoadImageFromFile(path, flipImage);

			if (image.GetRawData() == nullptr)
			{
				LOG_ERROR("Texture : at [{0}] couldn't be loaded", path.string());
			}

			m_filePath = std::filesystem::proximate(path).string();
			std::replace(m_filePath.begin(), m_filePath.end(), '\\', '/');

			m_format = format;
			m_width = image.GetWidth();
			m_height = image.GetHeight();
			m_textureType = GL_TEXTURE_2D;

			size_t channels = image.GetChannelCount();
			GLenum pixelType = image.IsFloatingPoint() ? GL_FLOAT : GL_UNSIGNED_BYTE;

			GLenum pixelFormat = GL_RGBA;


			switch (channels)
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
				LOG_ERROR("Texture : invalid channel count [{0}]", std::to_string(channels));
				break;
			}


			GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_textureId));
			GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GetGLFormat(m_format), (GLsizei)m_width, (GLsizei)m_height, 0, pixelFormat, pixelType, image.GetRawData()));

			if (image.GetRawData() != nullptr)
			{
				GenerateMipMaps();
			}
			else
			{
				GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			}

		}


		void Texture::Load(uint8_t* data, int width, int height, int channels, bool isFloating, TextureFormat format)
		{
			m_filePath = "rawtex";
			m_width = width;
			m_height = height;
			m_textureType = GL_TEXTURE_2D;
			m_format = format;

			GLenum type = isFloating ? GL_FLOAT : GL_UNSIGNED_BYTE;

			GLenum dataChannels = GL_RGB;

			switch (channels)
			{
			case 1:
				dataChannels = GL_RED;
				break;
			case 2:
				dataChannels = GL_RG;
				break;
			case 3:
				dataChannels = GL_RGB;
				break;
			case 4:
				dataChannels = GL_RGBA;
				break;
			default:
				LOG_ERROR("Texture : invalid channel count[{0}]", std::to_string(channels));
				break;
			}

			GLDEBUG(glBindTexture(GL_TEXTURE_2D, m_textureId));
			GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GetGLFormat(m_format), (GLsizei)width, (GLsizei)height, 0, dataChannels, type, data));

			if (data != nullptr)
			{
				GenerateMipMaps();
			}
			else
			{
				GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			}

		}

		void Texture::Load(const Image& image, TextureFormat format)
		{
			Load(image.GetRawData(), (int)image.GetWidth(), (int)image.GetHeight(), (int)image.GetChannelCount(), image.IsFloatingPoint(), format);
		}


		void Texture::LoadDepth(int width, int height, TextureFormat format)
		{
			m_filePath = "depthtex";
			m_width = width;
			m_height = height;
			m_textureType = GL_TEXTURE_2D;
			m_format = format;

			Bind();

			GLenum type = IsFloatingPoint() ? GL_FLOAT : GL_UNSIGNED_BYTE;

			GLDEBUG(glTexImage2D(GL_TEXTURE_2D, 0, GetGLFormat(format), width, height, 0, GL_DEPTH_COMPONENT, type, nullptr));
			//SetBorderColor(Vector4(1.0f));

			GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

		}

		void Texture::GenerateMipMaps()
		{
			Bind(0);
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLDEBUG(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLDEBUG(glGenerateMipmap(GL_TEXTURE_2D));
		}


		void Texture::SetBorderColor(Vector4 color)
		{
			Bind(0);

			Vector4 normalized = Clamp(color, Vector4(0.0f), Vector4(1.0f));

			GLDEBUG(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &normalized[0]));
		}


		bool Texture::IsMuliSampled() const
		{
			return m_textureType == GL_TEXTURE_2D_MULTISAMPLE;
		}

		bool Texture::IsFloatingPoint() const
		{
			switch (m_format)
			{
			case TextureFormat::R:
				return false;
			case TextureFormat::R16:
				return false;
			case TextureFormat::RG:
				return false;
			case TextureFormat::RG16:
				return false;
			case TextureFormat::R16F:
				return true;
			case TextureFormat::R32F:
				return true;
			case TextureFormat::RG16F:
				return true;
			case TextureFormat::RG32F:
				return true;
			case TextureFormat::RGB:
				return false;
			case TextureFormat::RGBA:
				return false;
			case TextureFormat::RGB16:
				return false;
			case TextureFormat::RGB16F:
				return true;
			case TextureFormat::RGBA16:
				return true;
			case TextureFormat::RGBA16F:
				return true;
			case TextureFormat::RGB32F:
				return true;
			case TextureFormat::RGBA32F:
				return true;
			case TextureFormat::DEPTH:
				return false;
			case TextureFormat::DEPTH32F:
				return true;
			default:
				return false;
			}

		}
		bool Texture::IsDepthOnly() const
		{
			return m_format == TextureFormat::DEPTH || m_format == TextureFormat::DEPTH32F;
		}

		size_t Texture::GetWidth() const
		{
			return m_width;
		}
		size_t Texture::GetHeight() const
		{
			return m_height;
		}
		size_t Texture::GetChannelCount() const
		{
			switch (m_format)
			{
			case TextureFormat::R:
				return 1;
			case TextureFormat::R16:
				return 1;
			case TextureFormat::RG:
				return 2;
			case TextureFormat::RG16:
				return 2;
			case TextureFormat::R16F:
				return 1;
			case TextureFormat::R32F:
				return 1;
			case TextureFormat::RG16F:
				return 2;
			case TextureFormat::RG32F:
				return 2;
			case TextureFormat::RGB:
				return 3;
			case TextureFormat::RGBA:
				return 4;
			case TextureFormat::RGB16:
				return 3;
			case TextureFormat::RGB16F:
				return 3;
			case TextureFormat::RGBA16:
				return 4;
			case TextureFormat::RGBA16F:
				return 4;
			case TextureFormat::RGB32F:
				return 3;
			case TextureFormat::RGBA32F:
				return 4;
			case TextureFormat::DEPTH:
				return 1;
			case TextureFormat::DEPTH32F:
				return 1;
			default:
				return 0;
			}
		}
		Vector4 Texture::GetBorderColor() const
		{
			Vector4 result = Vector4(0.0f);
			Bind(0);

			GLDEBUG(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &result[0]));
			return result;
		}

		size_t Texture::GetSampleCount() const
		{
			return m_samples;
		}

		size_t Texture::GetPixelSize() const
		{
			switch (m_format)
			{
			case TextureFormat::R:
				return 1;
			case TextureFormat::R16:
				return 2;
			case TextureFormat::RG:
				return 2;
			case TextureFormat::RG16:
				return 4;
			case TextureFormat::R16F:
				return 2;
			case TextureFormat::R32F:
				return 4;
			case TextureFormat::RG16F:
				return 4;
			case TextureFormat::RG32F:
				return 8;
			case TextureFormat::RGB:
				return 3;
			case TextureFormat::RGBA:
				return 4;
			case TextureFormat::RGB16:
				return 6;
			case TextureFormat::RGB16F:
				return 6;
			case TextureFormat::RGBA16:
				return 8;
			case TextureFormat::RGBA16F:
				return 8;
			case TextureFormat::RGB32F:
				return 12;
			case TextureFormat::RGBA32F:
				return 16;
			case TextureFormat::DEPTH:
				return 1;
			case TextureFormat::DEPTH32F:
				return 4;
			default:
				return 1;
			}
		}
		TextureFormat Texture::GetFormat() const
		{
			return m_format;
		}

		TextureWraping Texture::GetWraping() const
		{


			GLint result = 0;
			Bind(0);

			GLDEBUG(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &result));

			if (result == GL_REPEAT)
			{
				return TextureWraping::REPEAT;
			}
			else if (result == GL_CLAMP_TO_BORDER)
			{
				return TextureWraping::CLAMP_TO_BORDER;
			}
			else if (result == GL_MIRRORED_REPEAT)
			{
				return TextureWraping::MIRRORED_REPEAT;
			}

			return TextureWraping::CLAMP_TO_EDGE;
		}

		void Texture::SetWarping(TextureWraping wraping)
		{
			Bind(0);
			GLDEBUG(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &WrapTable[(int)wraping]));
			GLDEBUG(glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &WrapTable[(int)wraping]));
		}

		uint32_t Texture::GetTextureType() const
		{
			return m_textureType;
		}
    }
}