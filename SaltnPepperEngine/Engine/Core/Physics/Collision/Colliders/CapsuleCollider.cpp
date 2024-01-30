#include "CapsuleCollider.h"
#include "Engine/Core/Physics/PhysicsEngine/Rigidbody3D.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"


namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		CapsuleCollider::CapsuleCollider(float radius, float height)
			: m_radius(radius)
			, m_height(height)
		{
			m_transform = Matrix4(1.0f);
			m_type = ColliderType::CAPSULE;
		}

		
		std::vector<Vector3>& CapsuleCollider::GetCollisionNormals(const RigidBody3D* currentBody)
		{
			return m_normallist;
		} 

		std::vector<ColliderEdge>& CapsuleCollider::GetEdgeList(const RigidBody3D* currentBody)
		{
			return m_edgelist;
		}

		void CapsuleCollider::GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax)
		{
			Matrix4 transform = body ? body->GetTransform() * m_transform : m_transform;
			Vector3 position = transform[3];

			if (outMin)
			{
				*outMin = position - (axis * m_radius);
			}
			if (outMax)
			{
				*outMax = position - (axis * m_radius);
			}
		}

		void CapsuleCollider::GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const
		{
			manifoldPolygon.Faces[0] = currentObject->GetPosition() + axis * m_radius;
			manifoldPolygon.FaceCount = 1;

			manifoldPolygon.Normal = axis;
		}

		Matrix3 CapsuleCollider::BuildInverseInertia(float invMass) const
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

		void CapsuleCollider::DebugDraw(const RigidBody3D* currentBody) const
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

		void CapsuleCollider::SetRadius(float radius)
		{
			m_radius = radius;
			m_transform = Matrix4(1.0f);
		}

		float CapsuleCollider::GetRadius() const
		{
			return m_radius;
		}

		float CapsuleCollider::GetSize() const
		{
			return m_radius;
		}

		void CapsuleCollider::SetHeight(float height)
		{
			m_height = height;
			m_transform = Matrix4(1.0f);
			
		}

		float CapsuleCollider::GetHeight() const
		{
			return m_height;
		}
	}
}