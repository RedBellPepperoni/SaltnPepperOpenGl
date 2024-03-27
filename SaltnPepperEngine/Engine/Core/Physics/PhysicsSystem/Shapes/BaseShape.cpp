#include "BaseShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void BaseShape::DestroyShape()
		{
			// Clear pointers here
			if (collider != nullptr)
			{
				delete collider;
			}
		}


		void BaseShape::SetScale(const Vector3& scale)
		{
			collider->setLocalScaling(ToBulletVector3(scale));
		}

		const Vector3 BaseShape::GetScale() const
		{
			return FromBulletVector3(collider->getLocalScaling());
		}

		btCollisionShape* BaseShape::GetNativeHandle()
		{
			return collider;
		}

		BoundingBox BaseShape::GetAABB() const
		{
		
			Vector3 scale = GetScale();
			btVector3 min, max;
			btTransform bulletTransform;

			bulletTransform.setIdentity();

			collider->getAabb(bulletTransform, min, max);

			if (scale == Vector3{ 0.0f })
			{
				scale = Vector3{ 1.0f };
			}

			return BoundingBox{ FromBulletVector3(min) / scale,FromBulletVector3(max) / scale };

		}

		BoundingBox BaseShape::GetAABBTranformed(const Transform& transform) const
		{
			btVector3 min, max;
			btTransform bulletTransform;

			ToBulletTransform(transform,bulletTransform);

			collider->getAabb(bulletTransform, min, max);

			return BoundingBox{ FromBulletVector3(min),FromBulletVector3(max) };
		}

		BoundingSphere BaseShape::GetBoundingSphere() const
		{
			Vector3 scale = GetScale();
			btVector3 center;
			float radius;

			collider->getBoundingSphere(center, radius);

			if (scale == Vector3{ 0.0f })
			{
				scale = Vector3{ 1.0f };
			}

			radius /= (scale.x * RootThree<float>());

			return BoundingSphere{ FromBulletVector3(center),radius };
		
		}

		BoundingSphere BaseShape::GetBoundingSphereTransformed(const Transform& transform) const
		{
			btVector3 center;
			float radius;

			collider->getBoundingSphere(center, radius);

			return BoundingSphere{ FromBulletVector3(center) + transform.GetPosition(),radius / RootThree<float>()};

		}
		
	}
}