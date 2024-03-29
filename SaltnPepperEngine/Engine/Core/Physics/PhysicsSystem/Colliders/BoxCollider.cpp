#include "BoxCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void BoxCollider::CreateNewShape(const OrientedBoundingBox& box)
		{
			SetColliderChangedFlag(true);
			boxShape = MakeShared<BoxShape>(box);
			btCollisionShape* shape = boxShape->GetNativeHandle();
		}

		void BoxCollider::Init(BoundingBox box)
		{
			//CreateNewShape(OrientedBoundingBox());
			UpdateCollider(box);
		}

		void BoxCollider::UpdateCollider(BoundingBox box)
		{
			CreateNewShape(ToOBB(box));
		}

		BoxShape* BoxCollider::GetNativeHandle() const
		{
			return boxShape.get();
		}

		BoundingBox BoxCollider::GetAABB(const Transform& transform) const
		{
			return boxShape->GetAABBTranformed(transform);
		}

		OrientedBoundingBox BoxCollider::GetOBB(const Transform& transform) const
		{
			return boxShape->GetBoundingBoxTransformed(transform);
		}

		OrientedBoundingBox BoxCollider::GetOBBInternal() const
		{
			return GetNativeHandle()->GetBoundingBox();
		}

		BoundingSphere BoxCollider::GetBoundingSphere(const Transform& transform) const
		{
			return boxShape->GetBoundingSphereTransformed(transform);
		}

		void BoxCollider::SetBoundingBox(OrientedBoundingBox box)
		{
			CreateNewShape(box);
		}
	}
}