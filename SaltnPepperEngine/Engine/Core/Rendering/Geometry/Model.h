#ifndef MODEL_H
#define MODEL_H

#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Utils/Loading/ModelLoader.h"
#include "Engine/Core/Rendering/Geometry/Primitives.h"

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

	};

	
}



#endif // !MODEL_H


