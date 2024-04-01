#include "BoxCollider.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void BoxCollider::CreateNewShape(const BoundingBox& box)
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
			CreateNewShape((box));
		}

		BoxShape* BoxCollider::GetNativeHandle() const
		{
			return boxShape.get();
		}

		void BoxCollider::DebugDraw(Transform transform)
		{
			//BoundingBox debugBox = boxShape->GetBoundingBoxTransformed(transform); 

			btBoxShape* shape = static_cast<btBoxShape*>(boxShape->GetNativeHandle());
			
			int edgeCount = shape->getNumEdges();

			for (int i = 0; i < edgeCount; i++)
			{
				btVector3 pointOne;
				btVector3 pointTwo;

				shape->getEdge(i,pointOne,pointTwo);
				
				Rendering::DebugRenderer::DrawLine(FromBulletVector3(pointOne),FromBulletVector3(pointTwo),Vector4(0.0f,0.0f,1.0f,1.0f));
			}


			//Rendering::DebugRenderer::DebugDraw(debugBox, Vector4(0.0f, 0.0f, 1.0f, 1.0f),false);
		}

		BoundingBox BoxCollider::GetAABB(Transform& transform) const
		{
			return boxShape->GetAABBTranformed(transform);
		}

		BoundingBox BoxCollider::GetOBB(Transform& transform) const
		{
			return boxShape->GetBoundingBoxTransformed(transform);
		}

		BoundingBox BoxCollider::GetOBBInternal() const
		{
			return GetNativeHandle()->GetBoundingBox();
		}

		BoundingSphere BoxCollider::GetBoundingSphere(const Transform& transform) const
		{
			return boxShape->GetBoundingSphereTransformed(transform);
		}

		void BoxCollider::SetBoundingBox(BoundingBox box)
		{
			CreateNewShape(box);
		}
	}
}