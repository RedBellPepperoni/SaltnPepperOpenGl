#ifndef TEXTURE_H
#define TEXTURE_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Loading/FileSystem.h"
#include "Image.h"

namespace SaltnPepperEngine
{

    namespace Rendering
    {
        enum class TextureFormat : uint8_t
        {
            R,
            R16,
            RG,
            RG16,
            R16F,
            R32F,
            RG16F,
            RG32F,
            RGB,
            RGBA,
            RGB16,
            RGB16F,
            RGBA16,
            RGBA16F,
            RGB32F,
            RGBA32F,
            DEPTH,
            DEPTH32F
        };

        enum class TextureWraping : uint8_t
        {
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER,
            MIRRORED_REPEAT,
            REPEAT,
        };


        class Texture
        {
        public:



        private:

            std::string m_filePath;
            size_t m_width = 0;
            size_t m_height = 0;

            uint32_t m_textureId = 0;
            uint32_t m_textureType = 0;

            mutable int m_activeId = 0;

            TextureFormat m_format = TextureFormat::RGB;
            uint8_t m_samples;

            void DeleteTexture();


        public:




            Texture();
            ~Texture();


            Texture(const std::string& filePath, TextureFormat format);

            void Bind() const;
            void Bind(int id) const;
            void UnBind() const;
            int GetBoundId() const;
            uint32_t GetHandle() const;


            void Load(const FilePath& path, TextureFormat format);


            void Load(uint8_t* data, int width, int height, int channels, bool isFloating, TextureFormat format = TextureFormat::RGB);
            void Load(const Image& image, TextureFormat format = TextureFormat::RGB);

            void LoadDepth(int width, int height, TextureFormat format = TextureFormat::DEPTH);

            void GenerateMipMaps();

            void SetBorderColor(Vector4 color);
            bool IsMuliSampled() const;
            bool IsFloatingPoint() const;
            bool IsDepthOnly() const;


            size_t GetWidth() const;
            size_t GetHeight() const;
            size_t GetChannelCount() const;
            Vector4 GetBorderColor() const;
            size_t GetSampleCount() const;
            size_t GetPixelSize() const;

            TextureFormat GetFormat() const;
            TextureWraping GetWraping() const;

            void SetWarping(TextureWraping wraping);

            uint32_t GetTextureType() const;


            const std::string& GetFilePath() const;



        };
    }
}



#endif // !TEXTURE_H


