#ifndef COLOREDTEXTURES_H
#define COLOREDTEXTURES_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/utils/Maths/MathDefinitions.h"
#include "Texture.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
        class ColoredTexture
        {
        public:
            enum Palette
            {
                RED,
                GREEN,
                BLUE,
                YELLOW,
                BLACK,
                WHITE,
                FLAT_NORMAL,
                GREY,
            };

            static inline SharedPtr<Texture> MakeTexture(uint8_t r, uint8_t g, uint8_t b)
            {
                auto texture = MakeShared<Texture>();
                uint8_t buffer[] = { r, g, b };
                texture->Load(buffer, 1, 1, 3, false);
                texture->SetFilePath("coloredTexture");
                return texture;
            }

            static inline SharedPtr<Texture> MakeTexture(float r, float g, float b)
            {
                return MakeTexture(Vector3(r, g, b));
            }

            static inline SharedPtr<Texture>  MakeTexture(const Vector3& color)
            {
                uint8_t r = static_cast<uint8_t>(Clamp(color.r, 0.0f, 1.0f) * 255.0f);
                uint8_t g = static_cast<uint8_t>(Clamp(color.g, 0.0f, 1.0f) * 255.0f);
                uint8_t b = static_cast<uint8_t>(Clamp(color.b, 0.0f, 1.0f) * 255.0f);
                return MakeTexture(r, g, b);
            }

            static SharedPtr<Texture>  MakeTexture(Palette color)
            {
                return MakeTexture(Get(color));
            }

           
            static Vector3 Get(Palette color)
            {
                switch (color)
                {
                case ColoredTexture::RED:
                    return Vector3(1.0f, 0.0f, 0.0f);
                case ColoredTexture::GREEN:
                    return Vector3(0.0f, 1.0f, 0.0f);
                case ColoredTexture::BLUE:
                    return Vector3(0.0f, 0.0f, 1.0f);
                case ColoredTexture::YELLOW:
                    return Vector3(1.0f, 1.0f, 0.0f);
                case ColoredTexture::BLACK:
                    return Vector3(0.0f, 0.0f, 0.0f);
                case ColoredTexture::WHITE:
                    return Vector3(1.0f, 1.0f, 1.0f);
                case ColoredTexture::FLAT_NORMAL:
                    return Vector3(0.5f, 0.5f, 1.0f);
                case ColoredTexture::GREY:
                    return Vector3(0.5f, 0.5f, 0.5f);
                default:
                    return Vector3(0.0f);
                }
            }

            static Vector4 Get(Palette color, float transparency)
            {
                return Vector4(ColoredTexture::Get(color), transparency);
            }
        };
	}
}



#endif // !COLOREDTEXTURES_H

