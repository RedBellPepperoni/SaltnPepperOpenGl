#include "Material.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Resources/Resourcemanager.h"
#include "Engine/Utils/Logging/Log.h"
#include "Engine/Utils/Loading/FileSystem.h"


namespace SaltnPepperEngine
{
    namespace Rendering
    {
      


        SharedPtr<Texture> Material::GatherTexture(const std::string& filePath)
        {
            //Making sure no 2 textures have the same name or path
            std::string clampedname = FileSystem::GetFileName(filePath);

            SharedPtr<TextureLibrary>& texLibrary = Application::GetCurrent().GetTextureLibrary();

            SharedPtr<Texture> checktexture = texLibrary->GetResource(clampedname);

            if (checktexture == nullptr)
            {

                //checktexture = 
                // We truy to make a new resource
               checktexture = texLibrary->LoadTexture(clampedname,filePath, TextureFormat::RGBA);

                if (checktexture == nullptr)  
                {
                    LOG_ERROR("Material: Couldn't Create new Texture from filePath : {0}", filePath);
                    return nullptr;
                }

               // return checktexture;
            }

            return checktexture;
        }

        void Material::SetAlbedoTexture(const std::string& filePath)
        {
            textureMaps.albedoMap = GatherTexture(filePath);
        }

        void Material::SetAlbedoRef(SharedPtr<Texture>& texture)
        {
            textureMaps.albedoMap = texture;
        }



        void Material::SetNormalTexture(const std::string& filePath)
        {
            textureMaps.normalMap = GatherTexture(filePath);
        }

        void Material::SetMetallicTexture(const std::string& filePath)
        {
            textureMaps.metallicMap = GatherTexture(filePath);
        }

        void Material::SetAOTexture(const std::string& filePath)
        {
            textureMaps.aoMap = GatherTexture(filePath);
        }

        void Material::SetRoughnessTexture(const std::string& filePath)
        {
            textureMaps.roughnessMap = GatherTexture(filePath);
        }

        void Material::SetEmissiveTexture(const std::string& filePath)
        {
            textureMaps.emissiveMap = GatherTexture(filePath);
        }
    }
}