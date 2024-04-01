#include "CapsuleCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void CapsuleCollider::CreateNewShape(const BoundingCapsule& capsule)
		{
			SetColliderChangedFlag(true);
			capsuleShape = MakeShared<CapsuleShape>(capsule);
		}

		void CapsuleCollider::Init(const BoundingCapsule& capsule)
		{
			UpdateCollider(capsule);
		}

		void CapsuleCollider::UpdateCollider(const BoundingCapsule& capsule)
		{
			CreateNewShape(capsule);
		}

		CapsuleShape* CapsuleCollider::GetNativeHandle() const
		{
			return capsuleShape.get();
		}

		void CapsuleCollider::SetOrientation(const BoundingBox& aabb, BoundingCapsule::Axis axis)
		{
			CreateNewShape(ToCapsule(aabb, axis));
		}

		BoundingCapsule::Axis CapsuleCollider::GetOrientation() const
		{
			return capsuleShape->GetOrientation();
		}

		BoundingBox CapsuleCollider::GetAABB(const Transform& transform) const
		{
			return capsuleShape->GetAABBTranformed(transform);
		}

		BoundingSphere CapsuleCollider::GetBoundingSphere(const Transform& transform) const
		{
			return capsuleShape->GetBoundingSphereTransformed(transform);
		}

		BoundingCapsule CapsuleCollider::GetBoundingCapsule(const Transform& transform) const
		{
			return capsuleShape->GetBoundingCapsuleTransformed(transform);
		}

		BoundingCapsule CapsuleCollider::GetBoundingCapsuleInternal() const
		{
			return GetNativeHandle()->GetBoundingCapsule();
		}

		void CapsuleCollider::SetBoundingCapsule(BoundingCapsule capsule)
		{
			CreateNewShape(capsule );
		}
	}
}

