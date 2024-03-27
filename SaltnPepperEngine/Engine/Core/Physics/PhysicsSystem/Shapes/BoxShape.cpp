#include "BoxShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"


namespace SaltnPepperEngine
{

	namespace Physics
	{
		BoxShape::BoxShape(const OrientedBoundingBox& boundingBox)
		{
			CreateShape<btBoxShape>(ToBulletVector3(boundingBox.Length() * 0.5f));
		}

		BoxShape::BoxShape(BoxShape&& other) noexcept
		{
			collider = other.collider;
			other.collider = nullptr;
		}

		BoxShape& BoxShape::operator=(BoxShape&& other) noexcept
		{
			DestroyShape();
			collider = other.collider;
			other.collider = nullptr;
			return *this;
		}

		BoxShape::~BoxShape()
		{
			DestroyShape();
		}

		OrientedBoundingBox BoxShape::GetBoundingBoxTransformed(const Transform& transform) const
		{
			OrientedBoundingBox box = this->GetBoundingBox();
			box.min *= transform.GetScale();
			box.max *= transform.GetScale();
			box.center = transform.GetPosition();
			box.rotation = transform.GetRotation();
			return box;
			
		}

		OrientedBoundingBox BoxShape::GetBoundingBox() const
		{
			BoundingBox aabb = this->GetAABB();
			OrientedBoundingBox box = ToOBB(aabb);
			return box;
		}

		OrientedBoundingBox BoxShape::GetNativeBounding() const
		{
			return GetBoundingBox();
		}

		OrientedBoundingBox BoxShape::GetNativeBoundingTransformed(const Transform& transform) const
		{
			return GetBoundingBoxTransformed(transform);
		}
	}


}
