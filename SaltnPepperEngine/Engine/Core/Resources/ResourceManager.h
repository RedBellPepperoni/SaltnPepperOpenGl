#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <unordered_map>

namespace SaltnPepperEngine
{
	namespace Audio
	{
		struct AudioClip;
		class AudioManager;
	}


	namespace Rendering
	{
		class Shader;
		class Material;
		class CubeMap;

		enum class TextureFormat : uint8_t;
		class Texture;
		class SkinnedModel;
	}

	class Model;
	

	using Rendering::Shader;

	using Rendering::Material;
	using Rendering::SkinnedModel;

	using Rendering::Texture;
	using Rendering::TextureFormat;
	using Rendering::CubeMap;

	using Audio::AudioClip;
	using Audio::AudioManager;


	// generic Class to make Resource Libraries of different types of resources
	template <typename Type>
	class ResourceLibrary
	{
	public:


		typedef SharedPtr<Type> ResourcePtr;



		// cleaning out names : here the ResourceMap is defined as the friendlyName and the pointer to the resource
		using ResourceMap = std::unordered_map<std::string, ResourcePtr>;



		ResourceLibrary()
		{
		}


		~ResourceLibrary()
		{
		}



		void CreateResource(const std::string& friendlyname, ResourcePtr resource)
		{
			auto itr = m_resourcesMap.find(friendlyname);

			if (itr != m_resourcesMap.end())
			{
				// Incase we found the friendly name , we just update the resourcedata 
				itr->second = resource;

				// Add a Debug here Stating Resource Updated or some thing
				return;
			}

			// Inserting a new Reource pointer in the Library if the name is unique
			m_resourcesMap.emplace(friendlyname, resource);
		}


		void DestroyLibrary()
		{
			// Clearing all the Resource pointers
			m_resourcesMap.clear();
		}

		bool ResourceExists(const std::string& name)
		{
			typename ResourceMap::iterator itr = m_resourcesMap.find(name);
			return itr != m_resourcesMap.end();
		}

		ResourcePtr GetResource(std::string resourceName)
		{
			typename ResourceMap::iterator itr = m_resourcesMap.find(resourceName);

			if (itr == m_resourcesMap.end())
			{
				// No matching resource found we just give out a nullptr
				return nullptr;

			}

			// We found the resource... YAY!!!
			return itr->second;

		}




	protected:

		ResourceMap m_resourcesMap = {};


		//

	};

	//============================== MODEL LIBRARY =====================================



	// Definition for Model Resources
	class ModelLibrary : public ResourceLibrary<Model>
	{
	public:
		// Just defining the Constructor and Destructor
		ModelLibrary()
		{
		}

		virtual ~ModelLibrary()
		{
		}

		SharedPtr<Model> LoadModel(const std::string& friendlyName, const std::string& modelpath);
	};





	class ShaderLibrary : public ResourceLibrary<Shader>
	{



	public:



		ShaderLibrary()
		{

		}
		virtual ~ShaderLibrary()
		{

		}

		SharedPtr<Shader> LoadShader(const std::string& friendlyName, const std::string& vertexSource, const std::string& fragmentSource);

	};


	class MaterialLibrary : public ResourceLibrary<Material>
	{
	public:

		MaterialLibrary()
		{
		}

		virtual ~MaterialLibrary()
		{

		}

		SharedPtr<Material> CreateMaterial(const std::string& friendlyName);
	};

	class TextureLibrary : public ResourceLibrary<Texture>
	{
	public:
		TextureLibrary()
		{

		}

		virtual ~TextureLibrary()
		{

		}

		SharedPtr<Texture> LoadTexture(const std::string& friendlyName, const std::string& filePath, TextureFormat format);

	};

	class CubeMapLibrary : public ResourceLibrary<CubeMap>
	{
	public:
		CubeMapLibrary()
		{

		}

		virtual ~CubeMapLibrary()
		{

		}

		SharedPtr<CubeMap> LoadCubeMap(const std::string& friendlyName, const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);
	};

	class SkinnedModelLibrary : public ResourceLibrary<SkinnedModel>
	{
	public:

		SkinnedModelLibrary() = default;
		virtual ~SkinnedModelLibrary() = default;

		SharedPtr<SkinnedModel> LoadModel(const std::string& friendlyName,const std::string& filePath);
	};

	/*class AudioLibrary : public ResourceLibrary<AudioClip>
	{
	public:

		AudioLibrary() {};
		virtual ~AudioLibrary() {};

		SharedPtr<AudioClip> LoadAudio(const std::string friendlyName, const std::string& filePath);
	};*/


	template <typename Type>
	class Factory
	{
	public:
		typedef SharedPtr<Type> ResourcePointer;


		template <typename... Args>
		static ResourcePointer Create(Args&&... args)
		{
			ResourcePointer newResource = MakeShared<Type>(std::forward<Args>(args)...);

			return newResource;
		}
	};


}



#endif // !RESOURCEMANAGER_H


