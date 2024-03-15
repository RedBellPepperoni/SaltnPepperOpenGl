#ifndef SKINNEDMESH_H
#define SKINNEDMESH_H

#include "Engine/Core/Rendering/Buffers/VertexBuffer.h"
#include "Engine/Core/Rendering/Buffers/IndexBuffer.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

#include "Engine/Core/Rendering/RenderDefinitions.h"
#include <vector>
#include <string>


namespace SaltnPepperEngine
{
	class BoundingBox;

	namespace Rendering
	{
		class Material;

		//class SkinnedMesh
		//{
		//	std::string m_meshName;

		//	// Storage for vertices and indicies for the Mesh
		//	std::vector<SkinnedVertex> m_vertices;
		//	std::vector<uint32_t> m_indicies;

		//	// The Buffers that are needed for OpenGL
		//	SharedPtr<VertexBuffer> m_VBO;
		//	SharedPtr<IndexBuffer> m_IBO;


		//	uint32_t m_vertexCount;
		//	uint32_t m_indiciesCount;

		//	// Material Stuuff


		//	SharedPtr<Material> m_material;
		//	SharedPtr<BoundingBox> m_boundingBox;

		//public:

		//	SkinnedMesh();
		//	//Mesh(const Mesh& mesh);
		//	SkinnedMesh(const std::vector<SkinnedVertex>& inVertices, const std::vector<uint32_t>& inIndicies);

		//	~SkinnedMesh();

		//	// Change/Set the name of a mesh
		//	void SetName(const std::string& newName);

		//	// Getter for accessing the mesh's name
		//	const std::string& GetName() const;


		//	// Getters for Vertex and Index data
		//	const std::vector<SkinnedVertex>& GetVertexData();
		//	const std::vector<uint32_t>& GetIndexData();

		//	/*std::vector<Vertex>& GetVertexData();
		//	std::vector<uint32_t>& GetIndexData();*/

		//	void SetVertexData(const std::vector<Vertex> newVertexData);

		//	const uint32_t GetIndexCount() const;
		//	const uint32_t GetVertexCount() const;

		//	// Getters for Vertex and Index buffers
		//	SharedPtr<VertexBuffer>& GetVBO();
		//	SharedPtr<IndexBuffer>& GetIBO();

		//	void SetMaterial(const SharedPtr<Material>& newMaterial);
		//	SharedPtr<Material>& GetMaterial();

		//	const SharedPtr<BoundingBox>& GetBoundingBox();

		//	static void RecalculateNormals(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

		//};
	}
}

#endif // !SKINNEDMESH_H
