#include "Mesh.h"
#include "Engine/Core/Resources/ResourceManager.h"
#include "Engine/Core/Rendering/Material/Material.h"


namespace SaltnPepperEngine
{

    namespace Rendering
    {

		Mesh::Mesh()
			: m_vertices()
			, m_indicies()
			, m_VBO(nullptr)
			, m_IBO(nullptr)
			, m_meshName("empty")
		{
			m_material = MakeShared<Material>();
		}

		/*Mesh::Mesh(const Mesh& mesh)
			: m_vertices(mesh.m_vertices)
			, m_indicies(mesh.m_indicies)
			, m_VBO(mesh.m_VBO)
			, m_IBO(mesh.m_IBO)
			, m_meshName(mesh.GetName())
		{
		}*/

		Mesh::Mesh(const std::vector<Vertex>& inVertices, const std::vector<uint32_t>& inIndicies)
		{
			// copying the input Data into the vertex and index storage
			m_vertices = inVertices;
			m_indicies = inIndicies;

			// Calculating the total number of vertices and indices
			m_vertexCount = (uint32_t)m_vertices.size();
			m_indiciesCount = (uint32_t)m_indicies.size();

			// Creating a new Index buffer with the processed data
			m_IBO = Factory<IndexBuffer>::Create(m_indiciesCount, m_indicies.data(), UsageType::STATIC_COPY);

			// Creating a new vertex buffer with the processed data
			m_VBO = Factory<VertexBuffer>::Create(sizeof(Vertex) * m_vertexCount, m_vertices.data(), UsageType::STATIC_COPY);

			m_material = MakeShared<Material>();
		}

		// Default Constructor
		Mesh::~Mesh()
		{

		}

		void Mesh::SetName(const std::string& newName)
		{
			m_meshName = newName;
		}
		const std::string& Mesh::GetName() const
		{
			return m_meshName;
		}
		const std::vector<Vertex>& Mesh::GetVertexData()
		{
			return m_vertices;
		}
		const std::vector<uint32_t>& Mesh::GetIndexData()
		{
			return m_indicies;
		}

		void Mesh::SetVertexData(const std::vector<Vertex> newVertexData)
		{
			m_vertices = newVertexData;
		}

		const uint32_t Mesh::GetIndexCount() const
		{
			return m_indiciesCount;
		}

		const uint32_t Mesh::GetVertexCount() const
		{
			return m_vertexCount;
		}

		SharedPtr<VertexBuffer>& Mesh::GetVBO()
		{
			return m_VBO;
		}
		SharedPtr<IndexBuffer>& Mesh::GetIBO()
		{
			return m_IBO;
		}

		void Mesh::SetMaterial(const SharedPtr<Material>& newMaterial)
		{
			m_material = newMaterial;
		}

		SharedPtr<Material>& Mesh::GetMaterial() 
		{
			return m_material;
		}
		void Mesh::RecalculateNormals(std::vector<Vertex>& vertices,const std::vector<uint32_t>& indices)
		{
			std::vector<Vector3> normals = std::vector<Vector3>(vertices.size(), Vector3(0.0f));

			for (uint32_t i = 0; i < indices.size(); i += 3)
			{
				const int a = indices[i];
				const int b = indices[i + 1];
				const int c = indices[i + 2];

				const glm::vec3 _normal = Cross((vertices[b].position - vertices[a].position), (vertices[c].position - vertices[a].position));

				normals[a] += _normal;
				normals[b] += _normal;
				normals[c] += _normal;
			}

			for (uint32_t i = 0; i < vertices.size(); ++i)
			{
				Normalize(normals[i]);
				vertices[i].normal = normals[i];
			}
		}
    }
}
