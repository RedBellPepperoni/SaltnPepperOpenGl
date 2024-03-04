#include "ResourceManager.h"
#include "Engine/Utils/Logging/Log.h"

#include "Engine/Core/Rendering/Shader/Shader.h"
#include "Engine/Core/Rendering/Geometry/Model.h"
#include "Engine/Core/Rendering/Geometry/SkinnedModel.h"
#include "Engine/Core/Rendering/Geometry/SkinnedMesh.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Textures/CubeMap.h"





namespace SaltnPepperEngine
{
	SharedPtr<Shader> ShaderLibrary::LoadShader(const std::string& friendlyName, const std::string& vertexSource, const std::string& fragmentSource)
	{

		std::string path = FileSystem::GetCurrentPath().string();


		// Making a new Shader pointer ref
		SharedPtr<Shader> newShader = Factory<Shader>::Create();

		// Loading the Shader and creating and compiling a GL Shader
		if (newShader->Load(vertexSource, fragmentSource))
		{

			// adding to the Resource Map
			ShaderLibrary::CreateResource(friendlyName, newShader);
			// Retruning the newly created and compiled shader
			return newShader;

		}

		// A very rare case , but reseting it before deleting is essential 
		//newShader.reset();

		LOG_INFO("ShaderLibrary : Loaded [{0}]", friendlyName);

		//
		return nullptr;

	}


	SharedPtr<Model> ModelLibrary::LoadModel(const std::string& friendlyName, const std::string& modelpath)
	{
		SharedPtr<Model> newModel = Factory<Model>::Create(modelpath);

		// Model was loaded successfully
		if (newModel != nullptr)
		{
			ModelLibrary::CreateResource(friendlyName, newModel);
		}

		// Returning the model if created successfuly , nullptr otherwise
		return newModel;
	}



	SharedPtr<Material> MaterialLibrary::CreateMaterial(const std::string& friendlyName)
	{
		SharedPtr<Material> newMaterial = Factory<Material>::Create();

		if (newMaterial != nullptr)
		{
			MaterialLibrary::CreateResource(friendlyName, newMaterial);
		}

		return newMaterial;
	}

	SharedPtr<Texture> TextureLibrary::LoadTexture(const std::string& friendlyName, const std::string& filePath, TextureFormat format)
	{
		SharedPtr<Texture> newTexture = Factory<Texture>::Create(filePath, format);

		if (newTexture != nullptr)
		{
			TextureLibrary::CreateResource(friendlyName, newTexture);
		}


		return newTexture;
	}

	//SharedPtr<AudioClip> AudioLibrary::LoadAudio(const std::string friendlyName, const std::string& filePath)
	//{
	//	SharedPtr<AudioClip> newAudio = Factory<AudioClip>::Create(friendlyName, filePath);

	//	if (newAudio != nullptr)
	//	{
	//		AudioLibrary::CreateResource(friendlyName, newAudio);
	//	}

	//	return newAudio;
	//}

	SharedPtr<CubeMap> CubeMapLibrary::LoadCubeMap(const std::string& friendlyName, const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
	{
		SharedPtr<CubeMap> newCubeMap = Factory<CubeMap>::Create();

		if (newCubeMap != nullptr)
		{
			newCubeMap->Load(right, left, top, bottom, front, back);

			CubeMapLibrary::CreateResource(friendlyName, newCubeMap);
		}

		return newCubeMap;
	}


	SharedPtr<SkinnedModel> SkinnedModelLibrary::LoadModel(const std::string& friendlyName, const std::string& modelpath)
	{
		SharedPtr<SkinnedModel> newModel = Factory<SkinnedModel>::Create(modelpath);

		// Model was loaded successfully
		if (newModel != nullptr)
		{
			SkinnedModelLibrary::CreateResource(friendlyName, newModel);
		}

		// Returning the model if created successfuly , nullptr otherwise
		return newModel;
	}

}