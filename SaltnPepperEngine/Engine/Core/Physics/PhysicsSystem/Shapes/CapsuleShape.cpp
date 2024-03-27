#include "CapsuleShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"

using btCapsuleShapeY = btCapsuleShape;

namespace SaltnPepperEngine
{

	namespace Physics
	{
		CapsuleShape::CapsuleShape(const BoundingCapsule& capsule)
		{
			orientation = capsule.orientation;
			switch (capsule.orientation)
			{
			case BoundingCapsule::Axis::X:
				CreateShape<btCapsuleShapeX>(capsule.radius, capsule.height);
				break;

			case BoundingCapsule::Axis::Y:
				CreateShape<btCapsuleShapeY>(capsule.radius, capsule.height);
				break;

			case BoundingCapsule::Axis::Z:
				CreateShape<btCapsuleShapeZ>(capsule.radius, capsule.height);
				break;
			}
		}

		CapsuleShape::CapsuleShape(CapsuleShape&& other) noexcept
		{
			collider = other.collider;
			other.collider = nullptr;
		}

		CapsuleShape& CapsuleShape::operator=(CapsuleShape&& other) noexcept
		{
			DestroyShape();
		    collider = other.collider;
			other.collider = nullptr;
			return *this;
		}

		CapsuleShape::~CapsuleShape()
		{
			DestroyShape();
		}

		BoundingCapsule CapsuleShape::GetBoundingCapsuleTransformed(const Transform& transform) const
		{
			BoundingCapsule capsule = GetBoundingCapsule();
			Vector3 scale = transform.GetScale();
			capsule.center += transform.GetPosition();
			capsule.rotation = transform.GetRotation();

			switch (capsule.orientation)
			{
			case BoundingCapsule::Axis::X:
				capsule.height *= scale.x;
				capsule.radius *= (scale.y + scale.z) * 0.5f;
				break;

			case BoundingCapsule::Axis::Y:
				capsule.height *= scale.y;
				capsule.radius *= (scale.x + scale.z) * 0.5f;
				break;

			case BoundingCapsule::Axis::Z:
				capsule.height *= scale.z;
				capsule.radius *= (scale.x + scale.y) * 0.5f;
				break;
			}
			return capsule;
		}

		BoundingCapsule CapsuleShape::GetBoundingCapsule() const
		{
			BoundingBox box = GetAABB();
			auto scale = GetScale();
			box.m_min *= scale;
			box.m_max *= scale;

			BoundingCapsule capsule = ToCapsule(box, orientation);
			switch (orientation)
			{
			case BoundingCapsule::Axis::X:
				capsule.radius /= (scale.y + scale.z) * 0.5f;
				capsule.height /= scale.x;
				break;
			case BoundingCapsule::Axis::Y:
				capsule.radius /= (scale.x + scale.z) * 0.5f;
				capsule.height /= scale.y;
				break;
			case BoundingCapsule::Axis::Z:
				capsule.radius /= (scale.x + scale.y) * 0.5f;
				capsule.height /= scale.z;
				break;
			}

			return capsule;
		}

		BoundingCapsule CapsuleShape::GetNativeBounding() const
		{
			return GetBoundingCapsule();
		}

		BoundingCapsule CapsuleShape::GetNativeBoundingTransformed(const Transform& transform) const
		{
			return GetBoundingCapsuleTransformed(transform);
		}

		BoundingCapsule::Axis CapsuleShape::GetOrientation() const
		{
			return orientation;
		}
	}
}