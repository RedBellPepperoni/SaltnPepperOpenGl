#include "Image.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		Image::Image()
			: m_data(nullptr)
			, m_width(0)
			, m_height(0)
			, m_channels(0)
			, m_isFloatingPoint(false)
		{
		}


		Image::Image(uint8_t* data, size_t width, size_t height, size_t channels, bool isFloatingPoint)
			:m_data(data)
			, m_width(width)
			, m_height(height)
			, m_channels(channels)
			, m_isFloatingPoint(isFloatingPoint)
		{
		}

		Image::~Image()
		{
			Free();
		}

		Image::Image(Image&& other) noexcept
		{
			m_data = other.m_data;
			m_width = other.m_width;
			m_height = other.m_height;
			m_channels = other.m_channels;
			m_isFloatingPoint = other.m_isFloatingPoint;

			other.m_data = nullptr;
			other.m_width = other.m_height = other.m_channels = 0;
			other.m_isFloatingPoint = false;
		}


		Image& Image::operator=(Image&& other) noexcept
		{
			Free();
			m_data = other.m_data;
			m_width = other.m_width;
			m_height = other.m_height;
			m_channels = other.m_channels;
			m_isFloatingPoint = other.m_isFloatingPoint;

			other.m_data = nullptr;
			other.m_width = other.m_height = other.m_channels = 0;
			other.m_isFloatingPoint = false;
			return *this;

		}

		uint8_t* Image::GetRawData() const
		{
			return m_data;
		}

		size_t Image::GetWidth() const
		{
			return m_width;
		}

		size_t Image::GetHeight() const
		{
			return m_height;
		}

		size_t Image::GetChannelCount() const
		{
			return m_channels;
		}

		size_t Image::GetChannelSize() const
		{
			return m_isFloatingPoint ? sizeof(float) : sizeof(uint8_t);
		}

		size_t Image::GetPixelSize() const
		{
			return GetChannelCount() * GetChannelSize();
		}

		size_t Image::GetTotalByteSize() const
		{
			return GetHeight() * GetWidth() * GetPixelSize();
		}

		bool Image::IsFloatingPoint() const
		{
			return m_isFloatingPoint;
		}

		void Image::SetPixelByte(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			if (m_isFloatingPoint)
			{
				SetPixelFloat(x, y, (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f);
				return;
			}

			if (!(x < m_width && y < m_height))
			{
				LOG_ERROR("Image: Pixel size incorrect");
			}

			switch (m_channels)
			{
			case 1:
				m_data[x * m_height + y] = r;
				break;
			case 2:
				m_data[(x * m_height + y) * 2 + 0] = r;
				m_data[(x * m_height + y) * 2 + 1] = g;
				break;
			case 3:
				m_data[(x * m_height + y) * 3 + 0] = r;
				m_data[(x * m_height + y) * 3 + 1] = g;
				m_data[(x * m_height + y) * 3 + 2] = b;
				break;
			case 4:
				m_data[(x * m_height + y) * 4 + 0] = r;
				m_data[(x * m_height + y) * 4 + 1] = g;
				m_data[(x * m_height + y) * 4 + 2] = b;
				m_data[(x * m_height + y) * 4 + 3] = a;
				break;
			default:
				LOG_ERROR("Invalid Channel Count");
				break;
			}

		}

		template<typename T>
		T Clamp(T v, T minv, T maxv)
		{
			return (v < minv) ? minv : (v > maxv ? maxv : v);
		}


		void Image::SetPixelFloat(size_t x, size_t y, float r, float g, float b, float a)
		{
			if (m_isFloatingPoint)
			{
				SetPixelByte(x, y,
					(uint8_t)Clamp(r * 255.0f, 0.0f, 255.0f),
					(uint8_t)Clamp(g * 255.0f, 0.0f, 255.0f),
					(uint8_t)Clamp(b * 255.0f, 0.0f, 255.0f),
					(uint8_t)Clamp(a * 255.0f, 0.0f, 255.0f)
				);
				return;
			}

			if (!(x < m_width && y < m_height))
			{
				LOG_ERROR("Image: Pixel size incorrect");
			}

			switch (m_channels)
			{
			case 1:
				((float*)m_data)[x * m_height + y] = r;
				break;
			case 2:
				((float*)m_data)[(x * m_height + y) * 2 + 0] = r;
				((float*)m_data)[(x * m_height + y) * 2 + 1] = g;
				break;
			case 3:
				((float*)m_data)[(x * m_height + y) * 3 + 0] = r;
				((float*)m_data)[(x * m_height + y) * 3 + 1] = g;
				((float*)m_data)[(x * m_height + y) * 3 + 2] = b;
				break;
			case 4:
				((float*)m_data)[(x * m_height + y) * 4 + 0] = r;
				((float*)m_data)[(x * m_height + y) * 4 + 1] = g;
				((float*)m_data)[(x * m_height + y) * 4 + 2] = b;
				((float*)m_data)[(x * m_height + y) * 4 + 3] = a;
				break;
			default:
				LOG_ERROR("Invalid Channel Count");
				break;
			}
		}


		std::array<uint8_t, 4> Image::GetPixelByte(size_t x, size_t y) const
		{
			std::array<uint8_t, 4> rgba{ 0, 0, 0, 255 };
			if (m_isFloatingPoint)
			{
				auto pixel = GetPixelFloat(x, y);
				rgba[0] = (uint8_t)Clamp(pixel[0] * 255.0f, 0.0f, 255.0f);
				rgba[1] = (uint8_t)Clamp(pixel[1] * 255.0f, 0.0f, 255.0f);
				rgba[2] = (uint8_t)Clamp(pixel[2] * 255.0f, 0.0f, 255.0f);
				rgba[3] = (uint8_t)Clamp(pixel[3] * 255.0f, 0.0f, 255.0f);
				return rgba;
			}

			if (!(x < m_width && y < m_height))
			{
				LOG_ERROR("Image: Pixel size incorrect");
			}

			switch (m_channels)
			{
			case 1:
				rgba[0] = m_data[x * m_height + y];
				break;
			case 2:
				rgba[0] = m_data[(x * m_height + y) * 2 + 0];
				rgba[1] = m_data[(x * m_height + y) * 2 + 1];
				break;
			case 3:
				rgba[0] = m_data[(x * m_height + y) * 3 + 0];
				rgba[1] = m_data[(x * m_height + y) * 3 + 1];
				rgba[2] = m_data[(x * m_height + y) * 3 + 2];
				break;
			case 4:
				rgba[0] = m_data[(x * m_height + y) * 4 + 0];
				rgba[1] = m_data[(x * m_height + y) * 4 + 1];
				rgba[2] = m_data[(x * m_height + y) * 4 + 2];
				rgba[3] = m_data[(x * m_height + y) * 4 + 3];
				break;
			default:
				LOG_ERROR("Invalid Channel Count");
				break;
			}

			return rgba;
		}
		std::array<float, 4> Image::GetPixelFloat(size_t x, size_t y) const
		{
			std::array<float, 4> rgba{ 0.0f, 0.0f, 0.0f, 1.0f };

			if (!m_isFloatingPoint)
			{
				auto pixel = GetPixelByte(x, y);
				rgba[0] = (float)pixel[0] / 255.0f;
				rgba[1] = (float)pixel[1] / 255.0f;
				rgba[2] = (float)pixel[2] / 255.0f;
				rgba[3] = (float)pixel[3] / 255.0f;
				return rgba;
			}

			if (!(x < m_width && y < m_height))
			{
				LOG_ERROR("Image: Pixel size incorrect");
			}

			switch (m_channels)
			{
			case 1:
				rgba[0] = ((float*)m_data)[x * m_height + y];
				break;
			case 2:
				rgba[0] = ((float*)m_data)[(x * m_height + y) * 2 + 0];
				rgba[1] = ((float*)m_data)[(x * m_height + y) * 2 + 1];
				break;
			case 3:
				rgba[0] = ((float*)m_data)[(x * m_height + y) * 3 + 0];
				rgba[1] = ((float*)m_data)[(x * m_height + y) * 3 + 1];
				rgba[2] = ((float*)m_data)[(x * m_height + y) * 3 + 2];
				break;
			case 4:
				rgba[0] = ((float*)m_data)[(x * m_height + y) * 4 + 0];
				rgba[1] = ((float*)m_data)[(x * m_height + y) * 4 + 1];
				rgba[2] = ((float*)m_data)[(x * m_height + y) * 4 + 2];
				rgba[3] = ((float*)m_data)[(x * m_height + y) * 4 + 3];
				break;
			default:
				LOG_ERROR("Invalid Channel Count");
				break;
			}
			return rgba;
		}


		void Image::Free()
		{
			if (m_data != nullptr)
			{
				free(m_data);
			}
		}

	}
}