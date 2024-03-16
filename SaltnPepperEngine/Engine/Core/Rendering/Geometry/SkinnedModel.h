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


	static Matrix4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
	{
		Matrix4 to;
		to[0][0] = from->a1; to[1][0] = from->a2; to[2][0] = from->a3; to[3][0] = from->a4;
		to[0][1] = from->b1; to[1][1] = from->b2; to[2][1] = from->b3; to[3][1] = from->b4;
		to[0][2] = from->c1; to[1][2] = from->c2; to[2][2] = from->c3; to[3][2] = from->c4;
		to[0][3] = from->d1; to[1][3] = from->d2; to[2][3] = from->d3; to[3][3] = from->d4;
		return to;
		
	}
	
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

			inline std::vector<BoneProps>& GetBoneProps() { return m_boneProps; }

		private:

			void ExtractBoneWeightForVertices(std::vector<Vector4Int>& boneIDs_all, std::vector<Vector4>& weights_all, aiMesh* mesh, const aiScene* scene);
			
			void ProcessNode(aiNode* node, const aiScene* scene);
		
			

			SharedPtr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

		};
	}
}

#endif // !SKINNEDMODEL_H

