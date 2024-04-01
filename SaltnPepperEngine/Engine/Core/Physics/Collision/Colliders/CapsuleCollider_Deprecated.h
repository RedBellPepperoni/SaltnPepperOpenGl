#pragma once
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"

namespace SaltnPepperEngine
{ 
	namespace Physics
	{
		class CapsuleCollider_Deprecated : public Collider
		{

		public:
			CapsuleCollider_Deprecated(float radius = 1.0f, float height = 2.0f);
			~CapsuleCollider_Deprecated() = default;


			virtual std::vector<Vector3>& GetCollisionNormals(const RigidBody3D* currentBody) override;

			virtual std::vector<ColliderEdge>& GetEdgeList(const RigidBody3D* currentBody) override;


			// The Seperated Axis Theorem
			virtual void GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax) override;

			virtual void GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const override;

			virtual Matrix3 BuildInverseInertia(float invMass) const override;

			virtual void DebugDraw(const RigidBody3D* currentBody) const override;


			void SetRadius(float radius);
			float GetRadius() const;

			float GetSize() const override;

			void SetHeight(float height);
			float GetHeight() const;

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(m_radius, m_height);
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(m_radius, m_height);
				m_transform = Matrix4(1.0);
				m_type = ColliderType::CAPSULE;
			}

		protected:

			float m_radius = 1.0f;
			float m_height = 2.0f;


		};

	}
	
}

