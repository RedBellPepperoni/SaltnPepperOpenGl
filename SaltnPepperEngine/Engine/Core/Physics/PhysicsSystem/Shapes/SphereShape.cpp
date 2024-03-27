#include "SphereShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"

namespace SaltnPepperEngine
{

	namespace Physics
	{
		SphereShape::SphereShape(float radius)
		{
			CreateShape<btSphereShape>(radius);
		}

		SphereShape::SphereShape(const BoundingSphere& sphere)
			: SphereShape(sphere.GetRadius())
		{
		}

		SphereShape::SphereShape(SphereShape&& other) noexcept
		{
			collider = other.collider;
			other.collider = nullptr;
		}

		SphereShape& SphereShape::operator=(SphereShape&& other) noexcept
		{
			DestroyShape();
			collider = other.collider;
			other.collider = nullptr;
			return *this;
		}

		SphereShape::~SphereShape()
		{
			DestroyShape();
		}

		BoundingSphere SphereShape::GetNativeBounding() const
		{
			return GetBoundingSphere();
		}
		BoundingSphere SphereShape::GetNativeBoundingTransformed(const Transform& transform) const
		{
			return GetBoundingSphereTransformed(transform);
		}
	}
}