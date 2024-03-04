#include "SkinnedMesh.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Material/Material.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		SkinnedMesh::SkinnedMesh()
		{
		}

		SkinnedMesh::SkinnedMesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndicies, const std::vector<BoneInfluence>& vertexToBone, const std::map<std::string, uint32_t>& BoneNameIDMap)
		{
			m_vertices = inVertices;
			m_indicies = inIndicies;

			m_BoneData = vertexToBone;
			m_BoneNameToIndex = BoneNameIDMap;


			// Calculating the total number of vertices and indices
			m_vertexCount = (uint32_t)m_vertices.size();
			m_indiciesCount = (uint32_t)m_indicies.size();

			// Creating a new Index buffer with the processed data
			m_IBO = Factory<IndexBuffer>::Create(m_indiciesCount, m_indicies.data(), UsageType::STATIC_COPY);

			// Creating a new vertex buffer with the processed data
			m_VBO = Factory<VertexBuffer>::Create(sizeof(Vertex) * m_vertexCount, m_vertices.data(), UsageType::STATIC_COPY);


			m_boneVBO = Factory<VertexBuffer>::Create(sizeof(BoneInfluence) * m_BoneData.size(), m_BoneData.data(), UsageType::STATIC_COPY);

			m_material = MakeShared<Material>();
		}
	
		SkinnedMesh::~SkinnedMesh()
		{
		}
	}
}