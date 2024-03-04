#ifndef MODEL_H
#define MODEL_H

#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/Loading/ModelLoader.h"
#include "Engine/Core/Rendering/Geometry/Primitives.h"
#include <cereal/cereal.hpp>

namespace SaltnPepperEngine
{

	class Model
	{

	private:
		std::vector<SharedPtr<Mesh>> m_meshes;
		//std::vector<Transform> m_MeshTransform;

		PrimitiveType m_primitiveType = PrimitiveType::None;
		std::string m_filePath;


	public:

		Model() = default;
		Model(const FilePath& filepath);
		Model(PrimitiveType primitive);
		Model(SharedPtr<Mesh>& mesh, PrimitiveType primitive);


		std::vector<SharedPtr<Mesh>>& GetMeshes();

		void Load(const FilePath& filepath);


		const std::string& GetFilePath() const;
		PrimitiveType GetPrimitiveType();

		void SetPrimitiveType(PrimitiveType type);

        template <typename Archive>
        void save(Archive& archive) const
        {
            if (m_meshes.size() > 0)
            {
                std::string newPath = m_filePath;
  
                SharedPtr<Material> material = m_meshes.front()->GetMaterial();
               
               // std::unique_ptr<Rendering::Material> material = std::unique_ptr<Rendering::Material>(m_meshes.front()->GetMaterial().get());
                archive(cereal::make_nvp("PrimitiveType", m_primitiveType), cereal::make_nvp("FilePath", newPath), cereal::make_nvp("Material", material));
                //material.release();  
                
               
                //archive(cereal::make_nvp("PrimitiveType", m_primitiveType), cereal::make_nvp("FilePath", newPath));
                
            }
        }

        template <typename Archive>
        void load(Archive& archive)
        {
            //
            SharedPtr<Material> material = MakeShared<Rendering::Material>();

            //std::unique_ptr<Rendering::Material> material = std::unique_ptr<Rendering::Material>();
            archive(cereal::make_nvp("PrimitiveType", m_primitiveType), cereal::make_nvp("FilePath", m_filePath), cereal::make_nvp("Material", material));

            m_meshes.clear();

            if (m_primitiveType != PrimitiveType::External)
            {
                m_meshes.push_back(SharedPtr<Mesh>(GeneratePrimitive(m_primitiveType)));
                m_meshes.back()->SetMaterial(MakeShared<Material>());
                //material.release();
            }
            else
            {
                Load(m_filePath);
                // TODO: This should load material changes from editor
                // m_meshes.back()->SetMaterial(SharedPtr<Material>(material.get()));
                // material.release();
            }
        }

	};
  
	
}



#endif // !MODEL_H


