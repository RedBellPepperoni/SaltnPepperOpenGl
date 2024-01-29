#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Serialization/GLMSerialization.h"


#include "Engine/Core/Rendering/Textures/Texture.h"

namespace SaltnPepperEngine
{
    namespace Rendering
    {
      

        enum class MaterialType
        {
            Opaque,
            Transparent,
            Masked

        };


        struct MaterialTextures
        {
            SharedPtr<Texture> albedoMap;
            SharedPtr<Texture> normalMap;
            SharedPtr<Texture> metallicMap;
            SharedPtr<Texture> aoMap;
            SharedPtr<Texture> roughnessMap;
            SharedPtr<Texture> emissiveMap;
        };


        class Material
        {
        public:

            Material()
            {
                textureMaps.albedoMap = nullptr;
                textureMaps.normalMap = nullptr;
                textureMaps.metallicMap = nullptr;
                textureMaps.aoMap = nullptr;
                textureMaps.roughnessMap = nullptr;
                textureMaps.emissiveMap = nullptr;
            }
            ~Material() = default;


            std::string name = "DefaultMaterial";

            Vector2 uvMultiplier{ 1.0f };

            Vector4 albedoColour{ 0.0f,0.0f,0.0f,1.0f };
            float roughness = 0.8f;
            float metallic = 0.1f;
            float emissive = 0.0f;
            float reflectance = 0.0f;
            float ao = 1.0f;
            float normalMultiplier = 1.0f;
            float emissiveMultiplier = 1.0f;

            float albedoMapFactor = 1.0f;
            float roughnessMapFactor = 1.0f;
            float metallicMapFactor = 1.0f;
            float aoMapFactor = 1.0f;
            float normalMapFactor = 1.0f;
            float emissiveMapFactor = 1.0f;


            MaterialType type = MaterialType::Opaque;

            MaterialTextures textureMaps;

            constexpr static size_t TextureCount = 6;

            SharedPtr<Texture> GatherTexture(const std::string& filePath);

        public:

            const std::string& GetName() const { return name; }
            const MaterialType GetType() const { return type; }


            void SetAlbedoTexture(const std::string& filePath);
            void SetNormalTexture(const std::string& filePath);
            void SetMetallicTexture(const std::string& filePath);
            void SetAOTexture(const std::string& filePath);
            void SetRoughnessTexture(const std::string& filePath);
            void SetEmissiveTexture(const std::string& filePath);
        };
    }
}


#endif // !MATERIAL_H

