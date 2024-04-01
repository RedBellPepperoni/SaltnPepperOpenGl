#include "SphereCollider_Deprecated.h"
#include "Engine/Core/Physics/PhysicsEngine/Rigidbody3D.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		SphereCollider_Deprecated::SphereCollider_Deprecated()
		{
			m_type = ColliderType::SPHERE;
			SetRadius(1.0f);
			m_transform = Math::Scale(Matrix4(1.0f), Vector3(m_radius * 2.0f));
		}

		SphereCollider_Deprecated::SphereCollider_Deprecated(float radius)
		{
			m_type = ColliderType::SPHERE;
			SetRadius(radius);
			m_transform = Math::Scale(Matrix4(1.0f), Vector3(m_radius * 2.0f));

			
		}
		SphereCollider_Deprecated::~SphereCollider_Deprecated()
		{
		}

		std::vector<Vector3>& SphereCollider_Deprecated::GetCollisionNormals(const RigidBody3D* currentBody)
		{
			// NO need for this in sphere collision
			m_normallist.clear();
			return m_normallist;

		}
		std::vector<ColliderEdge>& SphereCollider_Deprecated::GetEdgeList(const RigidBody3D* currentBody)
		{
			return m_edgelist;
		}

		void SphereCollider_Deprecated::GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax)
		{
			Matrix4 transform = body ? body->GetTransform() * m_transform : m_transform;
		
			Vector3 position = transform[3];

			if (outMin)
			{
				*outMin = position - axis * m_radius;
			}

			if (outMax)
			{
				*outMax = position + axis * m_radius;
			}


		}

		void SphereCollider_Deprecated::GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const
		{
			// Since its a sphere, we only need one face that will be parallel

			manifoldPolygon.Faces[0] = currentObject->GetPosition() + axis * m_radius;
			manifoldPolygon.FaceCount = 1;

			manifoldPolygon.Normal = axis;
		}

		Matrix3 SphereCollider_Deprecated::BuildInverseInertia(float invMass) const
		{

		//https://en.wikipedia.org/wiki/List_of_moments_of_inertia
			// SOLID
			float i = 2.5f * invMass / (m_radius * m_radius);
			

			Matrix3 inertia;
			inertia[0][0] = i;
			inertia[1][1] = i;
			inertia[2][2] = i;

			return inertia;
		}

		void SphereCollider_Deprecated::DebugDraw(const RigidBody3D* currentBody) const
		{
			Matrix4 transform = currentBody->GetTransform() * m_transform;




			Vector4 position = transform[3];
			

			BoundingSphere sphere = BoundingSphere(position, m_radius);

			Vector3 pos = sphere.GetCenter();


			

			DebugRenderer::DebugDraw(sphere, Vector4(1.0f,1.0f,1.0f, 0.2f));
			DebugRenderer::DebugDrawSphere(m_radius, position, Vector4(1.0f,0.3f,1.0f,1.0f));

		}

		void SphereCollider_Deprecated::SetRadius(const float radius)
		{
			// Set the incoming radius
			m_radius = radius;

			// Set the scale
			m_transform = Math::Scale(Matrix4(1.0f), Vector3(m_radius * 2.0f));
		}

		const float SphereCollider_Deprecated::GetRadius() const
		{
			return m_radius;
		}
		float SphereCollider_Deprecated::GetSize() const
		{
			return m_radius;
		}
	}
}