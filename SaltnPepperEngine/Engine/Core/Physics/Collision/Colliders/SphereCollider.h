#pragma once
#include "Engine/Core/Physics/Collision/Colliders/Collider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

		class SphereCollider : public Collider
		{
		public:

			SphereCollider();

			//May be add an explicit here later on if it clashes
			 SphereCollider(float radius);


			~SphereCollider();


			virtual std::vector<Vector3>& GetCollisionNormals(const RigidBody3D* currentBody) override;

			virtual std::vector<ColliderEdge>& GetEdgeList(const RigidBody3D* currentBody) override;

			virtual void GetMinMaxFromAxis(const RigidBody3D* body, const Vector3& axis, Vector3* outMin, Vector3* outMax) override;

			// gets the face that is closest to the parallel plane of a given axis
			virtual void GetManifoldPolygon(const RigidBody3D* currentObject, const Vector3& axis, ManifoldPolygon& manifoldPolygon) const override;


			virtual Matrix3 BuildInverseInertia(float invMass) const override;

			virtual void DebugDraw(const RigidBody3D* currentBody) const override;


			void SetRadius(const float radius);

			const float GetRadius() const;

			float GetSize() const override;

			template <typename Archive>
			void save(Archive& archive) const
			{
				archive(m_radius);
			}

			template <typename Archive>
			void load(Archive& archive)
			{
				archive(m_radius);
				m_transform = Scale(Matrix4(1.0), Vector3(m_radius * 2.0f));
				m_type = ColliderType::SPHERE;
			}

		protected:

			// the radius of the Sphere Collider
			float m_radius;


		};



	}
}
