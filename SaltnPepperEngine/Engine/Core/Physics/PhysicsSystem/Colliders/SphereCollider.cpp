#include "SphereCollider.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

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

		void SphereCollider::DebugDraw(const Transform& transform)
		{
			const BoundingSphere debugSphere = sphereShape->GetBoundingSphereTransformed(transform);
			Rendering::DebugRenderer::DebugDraw(debugSphere, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
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