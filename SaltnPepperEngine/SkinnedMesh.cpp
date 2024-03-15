#include "SkinnedMesh.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"

namespace SaltnPepperEngine
{
	
	namespace Rendering
	{
		//SkinnedMesh::SkinnedMesh()
		//	: m_vertices()
		//	, m_indicies()
		//	, m_VBO(nullptr)
		//	, m_IBO(nullptr)
		//	, m_meshName("empty")
		//	, m_boundingBox(nullptr) {
		//}

		//SkinnedMesh::SkinnedMesh(const std::vector<SkinnedVertex>& inVertices, const std::vector<uint32_t>& inIndicies)
		//{
		//	// copying the input Data into the vertex and index storage
		//	m_vertices = inVertices;
		//	m_indicies = inIndicies;

		//	// Calculating the total number of vertices and indices
		//	m_vertexCount = (uint32_t)m_vertices.size();
		//	m_indiciesCount = (uint32_t)m_indicies.size();



		//	m_boundingBox = MakeShared<BoundingBox>();

		//	for (SkinnedVertex& vert : m_vertices)
		//	{
		//		m_boundingBox->Merge(vert.position);
		//	}

		//	// Creating a new Index buffer with the processed data
		//	m_IBO = Factory<IndexBuffer>::Create(m_indiciesCount, m_indicies.data(), UsageType::STATIC_COPY);

		//	// Creating a new vertex buffer with the processed data
		//	m_VBO = Factory<VertexBuffer>::Create(sizeof(Vertex) * m_vertexCount, m_vertices.data(), UsageType::STATIC_COPY);

		//	m_material = MakeShared<Material>();

		//}

		//SkinnedMesh::~SkinnedMesh()
		//{

		//}

		//void SkinnedMesh::SetName(const std::string& newName)
		//{
		//	m_meshName = newName;
		//}

		//const std::string& SkinnedMesh::GetName() const
		//{
		//	return m_meshName;
		//}

		//const std::vector<SkinnedVertex>& SkinnedMesh::GetVertexData()
		//{
		//	return m_vertices;
		//}
		//const std::vector<uint32_t>& SkinnedMesh::GetIndexData()
		//{
		//	// TODO: insert return statement here
		//}
		//void SkinnedMesh::SetVertexData(const std::vector<Vertex> newVertexData)
		//{
		//}
		//const uint32_t SkinnedMesh::GetIndexCount() const
		//{
		//	return 0;
		//}
		//const uint32_t SkinnedMesh::GetVertexCount() const
		//{
		//	return 0;
		//}
		//SharedPtr<VertexBuffer>& SkinnedMesh::GetVBO()
		//{
		//	// TODO: insert return statement here
		//}
		//SharedPtr<IndexBuffer>& SkinnedMesh::GetIBO()
		//{
		//	// TODO: insert return statement here
		//}
		//void SkinnedMesh::SetMaterial(const SharedPtr<Material>& newMaterial)
		//{
		//}
		//SharedPtr<Material>& SkinnedMesh::GetMaterial()
		//{
		//	// TODO: insert return statement here
		//}
		//const SharedPtr<BoundingBox>& SkinnedMesh::GetBoundingBox()
		//{
		//	// TODO: insert return statement here
		//}
		//void SkinnedMesh::RecalculateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
		//{
		//}
	}
}
