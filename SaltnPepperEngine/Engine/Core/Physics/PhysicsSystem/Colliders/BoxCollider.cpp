#include "BoxCollider.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

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

		void BoxCollider::DebugDraw(const Transform& transform)
		{
			BoundingBox debugSphere = boxShape->GetAABBTranformed(transform); 
			//debugSphere.Rotate(transform.GetRotation());

			Rendering::DebugRenderer::DebugDraw(debugSphere, Vector4(0.0f, 1.0f, 0.0f, 1.0f)); 
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