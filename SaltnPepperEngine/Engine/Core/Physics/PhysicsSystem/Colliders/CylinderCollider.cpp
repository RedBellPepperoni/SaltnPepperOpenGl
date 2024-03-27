
#include "CylinderCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void CylinderCollider::CreateNewShape(const BoundingCylinder& cylinder)
		{
			SetColliderChangedFlag(true);
			cylinderShape = MakeShared<CylinderShape>(cylinder);
		}

		void CylinderCollider::Init(const BoundingCylinder& cylinder)
		{
			UpdateCollider(cylinder);
		}

		void CylinderCollider::UpdateCollider(const BoundingCylinder& cylinder)
		{
			CreateNewShape(cylinder);
		}

		CylinderShape* CylinderCollider::GetNativeHandle() const
		{
			return cylinderShape.get();
		}

		void CylinderCollider::SetOrientation(const BoundingBox& aabb, BoundingCylinder::Axis axis)
		{
			CreateNewShape(ToCylinder(aabb, axis));
		}

		BoundingCylinder::Axis CylinderCollider::GetOrientation() const
		{
			return cylinderShape->GetOrientation();
		}

		BoundingBox CylinderCollider::GetAABB(const Transform& transform) const
		{
			return cylinderShape->GetAABBTranformed(transform);
		}

		BoundingSphere CylinderCollider::GetBoundingSphere(const Transform& transform) const
		{
			return cylinderShape->GetBoundingSphereTransformed(transform);
		}

		BoundingCylinder CylinderCollider::GetBoundingCylinder(const Transform& transform) const
		{
			return cylinderShape->GetBoundingCylinderTransformed(transform);
		}

		BoundingCylinder CylinderCollider::GetBoundingCylinderInternal() const
		{
			return GetNativeHandle()->GetBoundingCylinder();
		}

		void CylinderCollider::SetBoundingCylinder(BoundingCylinder cylinder)
		{
			CreateNewShape(cylinder);
		}
	}
}
