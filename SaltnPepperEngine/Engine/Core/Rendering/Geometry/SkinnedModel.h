#ifndef SKINNEDMODEL_H
#define SKINNEDMODEL_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <map>

namespace SaltnPepperEngine
{

	struct BoneProps
	{
		std::string name;
		Matrix4 offset;
	};

	
	namespace Rendering
	{
		class SkinnedModel
		{
			std::vector<SharedPtr<Mesh>> m_meshes;
			std::vector<BoneProps> m_boneProps;

			int m_boneCounter = 0;
			std::string m_filePath;



		public:

			SkinnedModel(const std::string& filePath);
			
			~SkinnedModel() = default;

			inline std::vector<SharedPtr<Mesh>>& GetMeshes() { return m_meshes; }

		private:

			void ExtractBoneWeightForVertices(std::vector<Vector4Int>& boneIDs_all, std::vector<Vector4>& weights_all, aiMesh* mesh, const aiScene* scene);
			
			void ProcessNode(aiNode* node, const aiScene* scene);
		
			static Matrix4 aiMatrix4x4ToGlm(const aiMatrix4x4* from);

			SharedPtr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

		};
	}
}

#endif // !SKINNEDMODEL_H

