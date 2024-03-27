#include "SphereCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void SphereCollider::CreateNewShape(const BoundingSphere& sphere)
		{
			SetColliderChangedFlag(true);
			sphereShape = MakeShared<SphereShape>(sphere.GetRadius());
		}

		void SphereCollider::Init(const BoundingSphere& sphere)
		{
			UpdateCollider(sphere);
		}

		void SphereCollider::UpdateCollider(const BoundingSphere& sphere)
		{
			CreateNewShape(sphere);
		}

		SphereShape* SphereCollider::GetNativeHandle() const
		{
			return sphereShape.get();
		}

		BoundingBox SphereCollider::GetAABB(const Transform& transform) const
		{
			return sphereShape->GetAABBTranformed(transform);
		}

		BoundingSphere SphereCollider::GetBoundingSphere(const Transform& transform) const
		{
			return sphereShape->GetBoundingSphereTransformed(transform);
		}

		BoundingSphere SphereCollider::GetBoundingSphereInternal() const
		{
			return GetNativeHandle()->GetBoundingSphere();
		}

		void SphereCollider::SetBoundingSphere(BoundingSphere sphere)
		{
			CreateNewShape(sphere);
		}
	}
}