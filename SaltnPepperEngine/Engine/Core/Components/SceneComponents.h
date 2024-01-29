#ifndef SCENECOMPONENTS_H
#define SCENECOMPONENTS_H

#include <string>
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Geometry/Model.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/UniqueId/UniqueId.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Logging/Log.h"
#include <cereal/cereal.hpp>


namespace SaltnPepperEngine
{


	//Forward Declaration

	class Model;

	namespace Rendering
	{
		class Mesh;
		class Material;
		enum class PrimitiveType : uint8_t;
		
	}

	using Rendering::Mesh;
	using Rendering::Material;
	using Rendering::PrimitiveType;

	namespace Components
	{


		struct IdComponent
		{
			// The Unique Identifier
			UniqueId ID;

			//Can Add saving and loading here
			template <typename Archive>
			void save(Archive& archive) const
			{
				uint64_t uuid = (uint64_t)ID.GetId();
				archive(uuid);
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				uint64_t uuid;
				archive(uuid);

				ID = UniqueId(uuid);
			}
		};



		struct ActiveComponent
		{
			template <typename Archive>
			void serialize(Archive& archive)
			{
				archive(cereal::make_nvp("Active", active));
			}

			ActiveComponent()
			{
				active = true;
			}

			ActiveComponent(bool newActive)
			{
				active = newActive;
			}

			bool active = true;


		};


		struct NameComponent
		{
			template <typename Archive>
			void serialize(Archive& archive)
			{
				archive(cereal::make_nvp("Name", name));
			}

			std::string name = "";

			
		};


		struct MeshComponent
		{


			MeshComponent();
			MeshComponent(const SharedPtr<Mesh>& meshRef);
			const SharedPtr<Mesh>& GetMesh();
			

			SharedPtr<Mesh> m_handle;
			bool isVisible = true;
			bool castsShadow = true;

		};


		struct ModelComponent
		{
			ModelComponent();
			ModelComponent(const std::string& filePath);
			ModelComponent(const SharedPtr<Model>& modelRef);
			ModelComponent(PrimitiveType type);


			void LoadLibraryModel(const std::string filePath);

			void LoadPrimitive(PrimitiveType type);


			SharedPtr<Model> m_handle;
			
			template <typename Archive>
			void save(Archive& archive) const
			{
				if (!m_handle || m_handle->GetMeshes().size() == 0)
					return;
				{
					std::string newPath;

					if (m_handle->GetPrimitiveType() == PrimitiveType::External)
						newPath = m_handle->GetFilePath();
					else
						newPath = "Primitive";

					// For now this saved material will be overriden by materials in the model file
					auto material = std::unique_ptr<Material>(m_handle->GetMeshes().front()->GetMaterial().get());
					archive(cereal::make_nvp("PrimitiveType", m_handle->GetPrimitiveType()), cereal::make_nvp("FilePath", newPath), cereal::make_nvp("Material", material));
					material.release();
				}
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				auto material = std::unique_ptr<Material>();

				std::string filePath;
				PrimitiveType primitiveType;

				archive(cereal::make_nvp("PrimitiveType", primitiveType), cereal::make_nvp("FilePath", filePath), cereal::make_nvp("Material", material));

				if (primitiveType != PrimitiveType::External)
				{
					m_handle = MakeShared<Model>(primitiveType);
					m_handle->GetMeshes().back()->SetMaterial(SharedPtr<Material>(material.get()));
					material.release();
				}
				else
				{
					LoadLibraryModel(filePath);
				}
			}

		};


		struct MeshRenderer
		{

			using MaterialRef = SharedPtr<Material>;
			using MaterialList = std::vector<MaterialRef>;




			MeshRenderer();
			MeshRenderer(MaterialRef material);
			MeshRenderer(MaterialList materials);
	

			MaterialRef GetMaterial();		
			MaterialList GetMaterials() const;
			MaterialRef GetMaterial(int index) const;
			

		private:

			MaterialList m_Materials;

		};

		struct PrefabComponent
		{
			PrefabComponent(const std::string& path)
			{
				Path = path;
			}

			std::string Path;
			
		};



	}

}



#endif // !SCENECOMPONENTS_H
