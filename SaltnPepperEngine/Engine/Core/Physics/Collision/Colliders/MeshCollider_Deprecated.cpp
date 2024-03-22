#include "MeshCollider_Deprecated.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Physics/PhysicsEngine/Rigidbody3D.h"

namespace SaltnPepperEngine
{


	namespace Physics
	{

	

		MeshCollider_Deprecated::MeshCollider_Deprecated()
		{
			m_HalfDimensions = Vector3(0.5f, 0.5f, 0.5f);
			m_type = ColliderType::MESH;
			m_normallist.resize(3);
		}

		MeshCollider_Deprecated::~MeshCollider_Deprecated()
		{
		}

		void MeshCollider_Deprecated::BuildFromMesh(Mesh* mesh)
		{
			m_Hull = MakeShared<ConvexHull_Deprecated>();

			SharedPtr<Rendering::VertexBuffer> vertexBuffer = mesh->GetVBO();
			
			
			std::vector<Rendering::Vertex> vertices = mesh->GetVertexData();
			std::vector<uint32_t> indices = mesh->GetIndexData();

			size_t indexCount = indices.size();

			for (const Rendering::Vertex& vert: vertices)
			{
				m_Hull->AddVertex(vert.position);
			}

			for (size_t i = 0; i < indexCount; i += 3)
			{
				Vector3 n1 = vertices[indices[i]].normal;  
				Vector3 n2 = vertices[indices[i + 1]].normal;
				Vector3 n3 = vertices[indices[i + 2]].normal;
				Vector3 normal = n1 + n2 + n3;
				normal = Normalize(normal);

				int vertexIdx[] = { (int)indices[i], (int)indices[i + 1], (int)indices[i + 2] };
				m_Hull->AddFace(normal, 3, vertexIdx);
			}

			m_edgelist.resize(m_Hull->GetNumEdges());

		}

		std::vector<Vector3>& MeshCollider_Deprecated::GetCollisionNormals(const RigidBody3D* body)
		{
			Matrix3 objOrientation = QuatToMatrix(body->GetRotation());
			m_normallist[0] = (objOrientation * Vector3(1.0f, 0.0f, 0.0f)); // X - Axis
			m_normallist[1] = (objOrientation * Vector3(0.0f, 1.0f, 0.0f)); // Y - Axis
			m_normallist[2] = (objOrientation * Vector3(0.0f, 0.0f, 1.0f)); // Z - Axis
		
			return m_normallist;
		
		}

		std::vector<ColliderEdge>& MeshCollider_Deprecated::GetEdgeList(const RigidBody3D* body)
		{
			Matrix4 transform = body->GetTransform() * m_transform;

			for (unsigned int i = 0; i < m_Hull->GetNumEdges(); ++i)
			{
				const HullEdge& edge = m_Hull->GetEdge(i);
				Vector3 A = transform * Vector4(m_Hull->GetVertex(edge.vStart).pos, 1.0f);
				Vector3 B = transform * Vector4(m_Hull->GetVertex(edge.vEnd).pos, 1.0f);

				m_edgelist[i] = { A, B };
			}

			return m_edgelist;
		}

		void MeshCollider_Deprecated::GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax)
		{

			Matrix4 wsTransform = Transpose(body ? body->GetTransform() * m_transform : m_transform);
			const Vector3 local_axis = wsTransform * Vector4(axis, 1.0f);

			int vMin, vMax;

			m_Hull->GetMinMaxVerticesInAxis(local_axis, &vMin, &vMax);

			if (outMin)
				*outMin = wsTransform * Vector4(m_Hull->GetVertex(vMin).pos, 1.0f);
			if (outMax)
				*outMax = wsTransform * Vector4(m_Hull->GetVertex(vMax).pos, 1.0f);
		}

		void MeshCollider_Deprecated::GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const
		{

			Matrix4 wsTransform = currentObject ? currentObject->GetTransform() * m_transform : m_transform;

			const Matrix3 invNormalMatrix = Inverse(wsTransform);
			const Matrix3 normalMatrix = Transpose(invNormalMatrix);

			const Vector3 local_axis = invNormalMatrix * axis;

			int minVertex, maxVertex;
			m_Hull->GetMinMaxVerticesInAxis(local_axis, &minVertex, &maxVertex);

			const HullVertex& vert = m_Hull->GetVertex(maxVertex);

			const HullFace* best_face = nullptr;
			float best_correlation = -FLT_MAX;
			for (int faceIdx : vert.enclosing_faces)
			{
				const HullFace* face = &m_Hull->GetFace(faceIdx);
				const float temp_correlation = Dot(local_axis, face->normal);
				if (temp_correlation > best_correlation)
				{
					best_correlation = temp_correlation;
					best_face = face;
				}
			}

			{
				if (best_face)
					manifoldPolygon.Normal = normalMatrix * best_face->normal;
				manifoldPolygon.Normal = Normalize(manifoldPolygon.Normal);
			}

			if (best_face)
			{
				for (int vertIdx : best_face->vert_ids)
				{
					const HullVertex& vertex = m_Hull->GetVertex(vertIdx);
					manifoldPolygon.Faces[manifoldPolygon.FaceCount++] = wsTransform * Vector4(vertex.pos, 1.0f);
				}
			}

			if (best_face)
			{
				// Add the reference face itself to the list of adjacent planes
				Vector3 wsPointOnPlane = wsTransform * Vector4(m_Hull->GetVertex(m_Hull->GetEdge(best_face->edge_ids[0]).vStart).pos, 1.0f);
				Vector3 planeNrml = -(normalMatrix * best_face->normal);
				planeNrml = Normalize(planeNrml);
				float planeDist = -Dot(planeNrml, wsPointOnPlane);

				manifoldPolygon.AdjacentPlanes[manifoldPolygon.PlaneCount++] = { planeNrml, planeDist };

				for (int edgeIdx : best_face->edge_ids)
				{
					const HullEdge& edge = m_Hull->GetEdge(edgeIdx);

					wsPointOnPlane = wsTransform * Vector4(m_Hull->GetVertex(edge.vStart).pos, 1.0f);

					for (int adjFaceIdx : edge.enclosing_faces)
					{
						if (adjFaceIdx != best_face->idx)
						{
							const HullFace& adjFace = m_Hull->GetFace(adjFaceIdx);

							planeNrml = -(normalMatrix * adjFace.normal);
							planeNrml = Normalize(planeNrml);
							planeDist = -Dot(planeNrml, wsPointOnPlane);

							manifoldPolygon.AdjacentPlanes[manifoldPolygon.PlaneCount++] = { planeNrml, planeDist };
						}
					}
				}
			}



		}

		Matrix3 MeshCollider_Deprecated::BuildInverseInertia(float invMass) const
		{
		//https://en.wikipedia.org/wiki/List_of_moments_of_inertia
			Matrix3 inertia(1.0f);

			Vector3 dimsSq = (m_HalfDimensions + m_HalfDimensions);
			dimsSq = dimsSq * dimsSq;

			inertia[0][0] = 12.f * invMass * 1.f / (dimsSq.y + dimsSq.z);
			inertia[1][1] = 12.f * invMass * 1.f / (dimsSq.x + dimsSq.z);
			inertia[2][2] = 12.f * invMass * 1.f / (dimsSq.x + dimsSq.y);

			return inertia;
		}

		void MeshCollider_Deprecated::DebugDraw(const RigidBody3D* currentBody) const
		{
			Matrix4 transform = currentBody->GetTransform() * m_transform;
			m_Hull->DebugDraw(transform);
		}

	}

}