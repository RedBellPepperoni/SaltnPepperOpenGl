#include "ImageLoader.h"
#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Utils/Logging/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace SaltnPepperEngine
{
    template<>
    Image ImageLoader::LoadImageFromFile(const FilePath& path, bool flipImage)
    {
        LOG_INFO("ImageLoader : Loading Image from file [{0}]", path.string());

        stbi_set_flip_vertically_on_load(flipImage);

        int width = 0;
        int height = 0;
        int channels = 0;

        uint8_t* data = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

        if (data == nullptr)
        {
            width = height = 0;
        }

        channels = 4;


        return Image(data, (size_t)width, (size_t)height, (size_t)channels, false);
    }


    Image ImageLoader::LoadImageFromMemory(const uint8_t* memory, size_t byteSize, bool flipImage)
    {

        LOG_INFO("ImageLoader : Loading Image from memory");

        stbi_set_flip_vertically_on_load(flipImage);

        int width = 0;
        int height = 0;
        int channels = 0;

        uint8_t* data = stbi_load_from_memory(memory, (int)byteSize, &width, &height, &channels, STBI_rgb_alpha);

        if (data == nullptr)
        {
            width = height = 0;
        }

        channels = 4;


        return Image(data, (size_t)width, (size_t)height, (size_t)channels, false);
    }
} 