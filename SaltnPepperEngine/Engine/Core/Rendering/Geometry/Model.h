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

		FilePath fileLocation;


	public:

		Model() = default;
		Model(const FilePath& filepath);
		Model(PrimitiveType primitive);

		std::vector<SharedPtr<Mesh>>& GetMeshes();

		void Load(const FilePath& filepath);

	};

	
}



#endif // !MODEL_H


