#ifndef MATERIAL_H
#define MATERIAL_H

#include "Engine/Core/System/Application/Application.h"
#include <string>
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Serialization/GLMSerialization.h"

#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include <cereal/cereal.hpp>

namespace SaltnPepperEngine
{
    namespace Rendering
    {
      

        enum class MaterialType
        {
            Opaque,
            Transparent,
            Masked,
            Custom,
            MaxNum

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

            Material(const MaterialTextures& materialtextures)
                : textureMaps(materialtextures)
            {
                
            }

            /*Material(const SharedPtr<Material>& material)
            {

            }*/

            ~Material() {};


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
            float normalMapFactor = 0.0f;
            float emissiveMapFactor = 1.0f;


            MaterialType m_type = MaterialType::Opaque;

            MaterialTextures textureMaps;

            constexpr static size_t TextureCount = 6;

            SharedPtr<Texture> GatherTexture(const std::string& filePath);

        public:

            const std::string& GetName() const { return name; }
            const MaterialType GetType() const { return m_type; }


            void SetAlbedoTexture(const std::string& filePath);
            void SetAlbedoRef(SharedPtr<Texture>& texture);
            void SetNormalTexture(const std::string& filePath);
            void SetMetallicTexture(const std::string& filePath);
            void SetAOTexture(const std::string& filePath);
            void SetRoughnessTexture(const std::string& filePath);
            void SetEmissiveTexture(const std::string& filePath);
        
        
            template <typename Archive>
            void save(Archive& archive) const
            {
                MaterialType mattype = m_type;
                std::string albedoFilePath = textureMaps.albedoMap ? textureMaps.albedoMap->GetFilePath() : "";
                std::string normalFilePath = textureMaps.normalMap ? textureMaps.normalMap->GetFilePath() : "";
                std::string metallicFilePath = textureMaps.metallicMap ? textureMaps.metallicMap->GetFilePath() : "";
                std::string roughnessFilePath = textureMaps.roughnessMap ? textureMaps.roughnessMap->GetFilePath() : "";
                std::string emissiveFilePath = textureMaps.emissiveMap ? textureMaps.emissiveMap->GetFilePath() : "";
                std::string aoFilePath = textureMaps.aoMap ? textureMaps.aoMap->GetFilePath() : "";

                

                archive(cereal::make_nvp("Albedo", albedoFilePath),
                    cereal::make_nvp("Normal", normalFilePath),
                    cereal::make_nvp("Metallic", metallicFilePath),
                    cereal::make_nvp("Roughness", roughnessFilePath),
                    cereal::make_nvp("Ao", aoFilePath),
                    cereal::make_nvp("Emissive", emissiveFilePath),
                    cereal::make_nvp("albedoColour", albedoColour),
                    cereal::make_nvp("roughnessValue", roughness),
                    cereal::make_nvp("metallicValue", metallic),
                    cereal::make_nvp("emissiveValue", emissive),
                    cereal::make_nvp("albedoMapFactor", albedoMapFactor),
                    cereal::make_nvp("metallicMapFactor", metallicMapFactor),
                    cereal::make_nvp("roughnessMapFactor", roughnessMapFactor),
                    cereal::make_nvp("normalMapFactor", normalMapFactor),
                    cereal::make_nvp("aoMapFactor", aoMapFactor),
                    cereal::make_nvp("emissiveMapFactor", emissiveMapFactor),
                    cereal::make_nvp("MaterialType", mattype)
                );
               

                archive(cereal::make_nvp("Reflectance", reflectance));
            }

            template <typename Archive>
            void load(Archive& archive)
            {
                std::string albedoFilePath;
                std::string normalFilePath;
                std::string roughnessFilePath;
                std::string metallicFilePath;
                std::string emissiveFilePath;
                std::string aoFilePath;
                std::string shaderFilePath;
                MaterialType mattype;

                archive(cereal::make_nvp("Albedo", albedoFilePath),
                    cereal::make_nvp("Normal", normalFilePath),
                    cereal::make_nvp("Metallic", metallicFilePath),
                    cereal::make_nvp("Roughness", roughnessFilePath),
                    cereal::make_nvp("Ao", aoFilePath),
                    cereal::make_nvp("Emissive", emissiveFilePath),
                    cereal::make_nvp("albedoColour", albedoColour),
                    cereal::make_nvp("roughnessValue", roughness),
                    cereal::make_nvp("metallicValue", metallic),
                    cereal::make_nvp("emissiveValue", emissive),
                    cereal::make_nvp("albedoMapFactor", albedoMapFactor),
                    cereal::make_nvp("metallicMapFactor", metallicMapFactor),
                    cereal::make_nvp("roughnessMapFactor", roughnessMapFactor),
                    cereal::make_nvp("normalMapFactor", normalMapFactor),
                    cereal::make_nvp("aoMapFactor", aoMapFactor),
                    cereal::make_nvp("emissiveMapFactor", emissiveMapFactor),
                    cereal::make_nvp("MaterialType", mattype)
                );
                 

             
                archive(cereal::make_nvp("Reflectance", reflectance));

                std::string fileName;

                m_type = mattype;

                // Albedo
                if (!albedoFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(albedoFilePath);
                    textureMaps.albedoMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, albedoFilePath, TextureFormat::RGBA);
                }
                // Normal
                if (!normalFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(normalFilePath);
                    textureMaps.normalMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, normalFilePath, TextureFormat::RGBA);
                } 
                // Metallic
                if (!metallicFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(metallicFilePath);
                    textureMaps.metallicMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, metallicFilePath, TextureFormat::RGBA);
                }
                // Roughness
                if (!roughnessFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(roughnessFilePath);
                    textureMaps.roughnessMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, roughnessFilePath, TextureFormat::RGBA);
                }
                // AO
                if (!aoFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(aoFilePath);
                    textureMaps.aoMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, aoFilePath, TextureFormat::RGBA);
                } 
                // Emissive
                if (!emissiveFilePath.empty())
                {
                    fileName = FileSystem::GetFileName(emissiveFilePath);
                    textureMaps.aoMap = Application::GetCurrent().GetTextureLibrary()->LoadTexture(fileName, emissiveFilePath, TextureFormat::RGBA);
                }


              
            }
        
        
        
        };
    }
}


#endif // !MATERIAL_H

