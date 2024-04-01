#include "BoxCollider_Deprecated.h"
#include "Engine/Core/Physics/PhysicsEngine/Rigidbody3D.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{

		SharedPtr<ConvexHull_Deprecated> BoxCollider_Deprecated::m_CubeHull = MakeShared<ConvexHull_Deprecated>();
		
		BoxCollider_Deprecated::BoxCollider_Deprecated()
		{
			m_CuboidHalfDimensions = Vector3(0.5f, 0.5f, 0.5f);
			m_type = ColliderType::BOX;
			m_transform = Scale(Matrix4(1.0), m_CuboidHalfDimensions);

			if (m_CubeHull->GetNumVertices() == 0)
			{
				ConstructCubeHull();
			}

			m_normallist.resize(3);
			m_edgelist.resize(m_CubeHull->GetNumEdges());
		}

		BoxCollider_Deprecated::BoxCollider_Deprecated(const Vector3& halfDims)
		{
			m_CuboidHalfDimensions = halfDims;
			m_type = ColliderType::BOX;
			m_transform = Scale(Matrix4(1.0), halfDims);

			if (m_CubeHull->GetNumVertices() == 0)
			{
				ConstructCubeHull();
			}

			m_normallist.resize(3);
			m_edgelist.resize(m_CubeHull->GetNumEdges());
			
		}
		BoxCollider_Deprecated::~BoxCollider_Deprecated()
		{
		}
		std::vector<Vector3>& BoxCollider_Deprecated::GetCollisionNormals(const RigidBody3D* currentBody)
		{
			// TODO: insert return statement here
			m_normallist.resize(3);

			Matrix3 objOrientation = glm::toMat3(currentBody->GetRotation());   //.RotationMatrix();
			m_normallist[0] = (objOrientation * Vector3(1.0f, 0.0f, 0.0f)); // X - Axis
			m_normallist[1] = (objOrientation * Vector3(0.0f, 1.0f, 0.0f)); // Y - Axis
			m_normallist[2] = (objOrientation * Vector3(0.0f, 0.0f, 1.0f)); // Z - Axis
		

			return m_normallist;
		}


		std::vector<ColliderEdge>& BoxCollider_Deprecated::GetEdgeList(const RigidBody3D* currentBody)
		{
			// TODO: insert return statement here
			Matrix4 transform = currentBody->GetTransform() * m_transform;
			for (unsigned int i = 0; i < m_CubeHull->GetNumEdges(); ++i)
			{
				const HullEdge& edge = m_CubeHull->GetEdge(i);
				Vector3 A = transform * Vector4(m_CubeHull->GetVertex(edge.vStart).pos, 1.0f);
				Vector3 B = transform * Vector4(m_CubeHull->GetVertex(edge.vEnd).pos, 1.0f);

				m_edgelist[i] = { A, B };
			}

			return m_edgelist;
		}
		void BoxCollider_Deprecated::GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax)
		{

			Matrix4 wsTransform = body ? body->GetTransform() * m_transform : m_transform;
			const Vector3 local_axis = Transpose(wsTransform) * Vector4(axis, 1.0f);

			int vMin, vMax;

			m_CubeHull->GetMinMaxVerticesInAxis(local_axis, &vMin, &vMax);

			if (outMin)
				*outMin = wsTransform * Vector4(m_CubeHull->GetVertex(vMin).pos, 1.0f);
			if (outMax)
				*outMax = wsTransform * Vector4(m_CubeHull->GetVertex(vMax).pos, 1.0f);
		}

		void BoxCollider_Deprecated::GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const
		{
			Matrix4 wsTransform = currentObject ? currentObject->GetTransform() * m_transform : m_transform;

			const Matrix3 invNormalMatrix = Inverse(Matrix3(wsTransform));
			const Matrix3 normalMatrix = Transpose(invNormalMatrix);

			const Vector3 local_axis = invNormalMatrix * axis;

			int minVertex, maxVertex;
			m_CubeHull->GetMinMaxVerticesInAxis(local_axis, &minVertex, &maxVertex);

			const HullVertex& vert = m_CubeHull->GetVertex(maxVertex);

			const HullFace* best_face = nullptr;
			float best_correlation = -FLT_MAX;
			for (int faceIdx : vert.enclosing_faces)
			{
				const HullFace* face = &m_CubeHull->GetFace(faceIdx);
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
					const HullVertex& vertex = m_CubeHull->GetVertex(vertIdx);
					manifoldPolygon.Faces[manifoldPolygon.FaceCount++] = wsTransform * Vector4(vertex.pos, 1.0f);
				}
			}

			if (best_face)
			{
				// Add the reference face itself to the list of adjacent planes
				Vector3 wsPointOnPlane = wsTransform * Vector4(m_CubeHull->GetVertex(m_CubeHull->GetEdge(best_face->edge_ids[0]).vStart).pos, 1.0f);
				Vector3 planeNrml = -(normalMatrix * best_face->normal);
				planeNrml = Normalize(planeNrml);
				float planeDist = -Dot(planeNrml, wsPointOnPlane);

				manifoldPolygon.AdjacentPlanes[manifoldPolygon.PlaneCount++] = Plane(planeNrml, planeDist);

				for (int edgeIdx : best_face->edge_ids)
				{
					const HullEdge& edge = m_CubeHull->GetEdge(edgeIdx);

					wsPointOnPlane = wsTransform * Vector4(m_CubeHull->GetVertex(edge.vStart).pos, 1.0f);

					for (int adjFaceIdx : edge.enclosing_faces)
					{
						if (adjFaceIdx != best_face->idx)
						{
							const HullFace& adjFace = m_CubeHull->GetFace(adjFaceIdx);

							planeNrml = -(normalMatrix * adjFace.normal);
							planeNrml = Normalize(planeNrml);
							planeDist = -Dot(planeNrml, wsPointOnPlane);

							manifoldPolygon.AdjacentPlanes[manifoldPolygon.PlaneCount++] = Plane(planeNrml, planeDist);
						}
					}
				}
			}
		}

		Matrix3 BoxCollider_Deprecated::BuildInverseInertia(float invMass) const
		{

		//https://en.wikipedia.org/wiki/List_of_moments_of_inertia
			Matrix3 inertia(1.0f);

			Vector3 dimsSq = (m_CuboidHalfDimensions + m_CuboidHalfDimensions);
			dimsSq = dimsSq * dimsSq;

			inertia[0][0] = 12.f * invMass * 1.f / (dimsSq.y + dimsSq.z);
			inertia[1][1] = 12.f * invMass * 1.f / (dimsSq.x + dimsSq.z);
			inertia[2][2] = 12.f * invMass * 1.f / (dimsSq.x + dimsSq.y);

			return inertia;
		}

		void BoxCollider_Deprecated::DebugDraw(const RigidBody3D* currentBody) const
		{
			Matrix4 transform = currentBody->GetTransform() * m_transform;


			if (m_CubeHull->GetNumVertices() == 0)
			{
				ConstructCubeHull();
			}

			m_CubeHull->DebugDraw(transform);
		}

		void BoxCollider_Deprecated::ConstructCubeHull()
		{
			
			// Vertices
			m_CubeHull->AddVertex(Vector3(-1.0f, -1.0f, -1.0f)); // 0
			m_CubeHull->AddVertex(Vector3(-1.0f, 1.0f, -1.0f));  // 1
			m_CubeHull->AddVertex(Vector3(1.0f, 1.0f, -1.0f));   // 2
			m_CubeHull->AddVertex(Vector3(1.0f, -1.0f, -1.0f));  // 3

			m_CubeHull->AddVertex(Vector3(-1.0f, -1.0f, 1.0f)); // 4
			m_CubeHull->AddVertex(Vector3(-1.0f, 1.0f, 1.0f));  // 5
			m_CubeHull->AddVertex(Vector3(1.0f, 1.0f, 1.0f));   // 6
			m_CubeHull->AddVertex(Vector3(1.0f, -1.0f, 1.0f));  // 7

			int face1[] = { 0, 1, 2, 3 };
			int face2[] = { 7, 6, 5, 4 };
			int face3[] = { 5, 6, 2, 1 };
			int face4[] = { 0, 3, 7, 4 };
			int face5[] = { 6, 7, 3, 2 };
			int face6[] = { 4, 5, 1, 0 };

			// Faces
			m_CubeHull->AddFace(Vector3(0.0f, 0.0f, -1.0f), 4, face1);
			m_CubeHull->AddFace(Vector3(0.0f, 0.0f, 1.0f), 4, face2);
			m_CubeHull->AddFace(Vector3(0.0f, 1.0f, 0.0f), 4, face3);
			m_CubeHull->AddFace(Vector3(0.0f, -1.0f, 0.0f), 4, face4);
			m_CubeHull->AddFace(Vector3(1.0f, 0.0f, 0.0f), 4, face5);
			m_CubeHull->AddFace(Vector3(-1.0f, 0.0f, 0.0f), 4, face6);
		}
	}
}