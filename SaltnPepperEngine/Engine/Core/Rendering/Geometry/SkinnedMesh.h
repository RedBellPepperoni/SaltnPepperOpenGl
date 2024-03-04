#ifndef SKINNED_MESH_H
#define SKINNED_MESH_H

#include "Mesh.h"
#include <map>
#include <unordered_map>

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class SkinnedMesh : public Mesh
		{
		public:

			SkinnedMesh(); 

			SkinnedMesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndicies, const std::vector<BoneInfluence>& vertexToBone, const std::map<std::string,uint32_t>& BoneNameIDMap);
			virtual ~SkinnedMesh();

			const uint32_t GetBoneCount() const { return (uint32_t)m_BoneNameToIndex.size(); }

		protected:

			SharedPtr<VertexBuffer> m_boneVBO;
			std::vector<BoneInfluence> m_BoneData;
			std::map<std::string, uint32_t> m_BoneNameToIndex;

		};
	}

}

#endif // !SKINNED_MESH_H


