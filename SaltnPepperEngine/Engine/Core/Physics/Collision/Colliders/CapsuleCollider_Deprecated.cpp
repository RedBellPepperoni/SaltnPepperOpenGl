#include "CapsuleCollider_Deprecated.h"
#include "Engine/Core/Physics/PhysicsEngine/Rigidbody3D.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"


namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		CapsuleCollider_Deprecated::CapsuleCollider_Deprecated(float radius, float height)
			: m_radius(radius)
			, m_height(height)
		{
			m_transform = Matrix4(1.0f);
			m_type = ColliderType::CAPSULE;
		}

		
		std::vector<Vector3>& CapsuleCollider_Deprecated::GetCollisionNormals(const RigidBody3D* currentBody)
		{
			return m_normallist;
		} 

		std::vector<ColliderEdge>& CapsuleCollider_Deprecated::GetEdgeList(const RigidBody3D* currentBody)
		{
			return m_edgelist;
		}

		void CapsuleCollider_Deprecated::GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax)
		{
			Matrix4 transform = body ? body->GetTransform() * m_transform : m_transform;
			Vector3 pos = transform[3];

			Vector3 topPosition = Vector3(transform * Vector4(0.0f, m_height * 0.5f, 0.0f, 1.0f));
			Vector3 bottomPosition = Vector3(transform * Vector4(0.0f, -m_height * 0.5f, 0.0f, 1.0f));

			// Transform the axis into the local coordinate space of the capsule
			Matrix4 inverseTransform = glm::affineInverse(transform);
			Vector3 localAxis = Vector3(inverseTransform * Vector4(axis, 1.0f));

			float minProj = Dot(topPosition, localAxis) - m_radius;
			float maxProj = Dot(bottomPosition, localAxis) + m_radius;

			*outMin = topPosition + minProj * localAxis;
			*outMax = bottomPosition + maxProj * localAxis;
		}

		void CapsuleCollider_Deprecated::GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const
		{
			manifoldPolygon.Faces[0] = currentObject->GetPosition() + axis * m_radius;
			manifoldPolygon.FaceCount = 1;

			manifoldPolygon.Normal = axis;
		}

		Matrix3 CapsuleCollider_Deprecated::BuildInverseInertia(float invMass) const
		{
			Vector3 halfExtents(m_radius, m_radius, m_radius);
			halfExtents.x += m_height * 0.5f;

			float lx = 2.0f * (halfExtents.x);
			float ly = 2.0f * (halfExtents.y);
			float lz = 2.0f * (halfExtents.z);
			const float x2 = lx * lx;
			const float y2 = ly * ly;
			const float z2 = lz * lz;
			const float scaledmass = (1.0f / invMass) * float(.08333333);

			Matrix3 inertia(1.0f);

			inertia[0][0] = 1.0f / scaledmass * (y2 + z2);
			inertia[1][1] = 1.0f / scaledmass * (x2 + z2);
			inertia[2][2] = 1.0f / scaledmass * (x2 + y2);

			return inertia;
		}

		void CapsuleCollider_Deprecated::DebugDraw(const RigidBody3D* currentBody) const
		{
			Matrix4 transform = currentBody->GetTransform() * m_transform;

			Vector3 translation;
			Vector3 skew;
			Vector4 perspective;
			Vector3 scale;
			Quaternion rotation;

			Decompose(transform, scale, rotation, translation, skew, perspective);

			DebugRenderer::DebugDrawCapsule(translation, rotation, m_height, m_radius, Vector4(1.0f, 1.0f, 0.0f, 1.0f));

		}

		void CapsuleCollider_Deprecated::SetRadius(float radius)
		{
			m_radius = radius;
			m_transform = Matrix4(1.0f);
		}

		float CapsuleCollider_Deprecated::GetRadius() const
		{
			return m_radius;
		}

		float CapsuleCollider_Deprecated::GetSize() const
		{
			return m_radius;
		}

		void CapsuleCollider_Deprecated::SetHeight(float height)
		{
			m_height = height;
			m_transform = Matrix4(1.0f);
			
		}

		float CapsuleCollider_Deprecated::GetHeight() const
		{
			return m_height;
		}
	}
}