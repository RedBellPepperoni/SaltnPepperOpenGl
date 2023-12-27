#ifndef IMAGE_H
#define IMAGE_H
#include <cstddef>
#include <cstdint>
#include <array>

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class Image
		{
		public:

			Image();
			Image(uint8_t* data, size_t width, size_t height, size_t channels, bool isFloatingPoint);

			~Image();

			Image(const Image& other) = delete;
			Image& operator=(const Image& other) = delete;
			Image(Image&& other) noexcept;
			Image& operator=(Image&& other) noexcept;

			uint8_t* GetRawData() const;
			size_t GetWidth() const;
			size_t GetHeight() const;
			size_t GetChannelCount() const;
			size_t GetChannelSize() const;
			size_t GetPixelSize() const;
			size_t GetTotalByteSize() const;
			bool IsFloatingPoint() const;

			void SetPixelByte(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
			void SetPixelFloat(size_t x, size_t y, float r, float g, float b, float a = 1.0f);
			std::array<uint8_t, 4> GetPixelByte(size_t x, size_t y) const;
			std::array<float, 4> GetPixelFloat(size_t x, size_t y) const;


		private:

			void Free();


		private:

			uint8_t* m_data;
			size_t m_width;
			size_t m_height;
			uint8_t m_channels;
			bool m_isFloatingPoint;


		};
	}
}



#endif // !IMAGE_H

