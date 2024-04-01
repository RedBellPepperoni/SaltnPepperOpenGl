#include "CylinderShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"

using btCylinderShapeY = btCylinderShape;

namespace SaltnPepperEngine
{

	namespace Physics
	{
		CylinderShape::CylinderShape(const BoundingCylinder& cylinder)
		{
			btVector3 boundings{ 0.0f, 0.0f, 0.0f };
			orientation = cylinder.orientation;

			switch (cylinder.orientation)
			{
			case BoundingCylinder::Axis::X:
				boundings.setValue(cylinder.height * 0.5f, cylinder.radiusX, cylinder.radiusZ);
				CreateShape<btCylinderShapeX>(boundings);
				break;
			case BoundingCylinder::Axis::Y:
				boundings.setValue(cylinder.radiusX, cylinder.height * 0.5f, cylinder.radiusZ);
				CreateShape<btCylinderShapeY>(boundings);
				break;
			case BoundingCylinder::Axis::Z:
				boundings.setValue(cylinder.radiusX, cylinder.radiusZ, cylinder.height * 0.5f);
				CreateShape<btCylinderShapeZ>(boundings);
				break;
			}
		}

		CylinderShape::CylinderShape(CylinderShape&& other) noexcept
		{
			collider = other.collider;
			other.collider = nullptr;
		}

		CylinderShape& CylinderShape::operator=(CylinderShape&& other) noexcept
		{
			DestroyShape();
			collider = other.collider;
			other.collider = nullptr;
			return *this;
		}

		CylinderShape::~CylinderShape()
		{
			DestroyShape();
		}

		BoundingCylinder CylinderShape::GetBoundingCylinderTransformed(const Transform& transform) const
		{
			BoundingCylinder cylinder = GetBoundingCylinder();
			Vector3 scale = transform.GetScale();

			cylinder.center += transform.GetPosition();
			cylinder.rotation = transform.GetRotation();

			switch (cylinder.orientation)
			{
			case BoundingCylinder::Axis::X:
				cylinder.height *= scale.x;
				cylinder.radiusX *= scale.y;
				cylinder.radiusZ *= scale.z;
				break;

			case BoundingCylinder::Axis::Y:
				cylinder.height *= scale.y;
				cylinder.radiusX *= scale.x;
				cylinder.radiusZ *= scale.z;
				break;

			case BoundingCylinder::Axis::Z:
				cylinder.height *= scale.z;
				cylinder.radiusX *= scale.x;
				cylinder.radiusZ *= scale.y;
				break;
			}

			return cylinder;
		}

		BoundingCylinder CylinderShape::GetBoundingCylinder() const
		{
			BoundingBox box = GetAABB();
			BoundingCylinder cylinder = ToCylinder(box, orientation);
			return cylinder;
		}

		BoundingCylinder CylinderShape::GetNativeBoundingTransformed(const Transform& transform) const
		{
			return GetBoundingCylinderTransformed(transform);
		}

		BoundingCylinder CylinderShape::GetNativeBounding() const
		{
			return GetBoundingCylinder();
		}

		BoundingCylinder::Axis CylinderShape::GetOrientation() const
		{
			return orientation;
		}
	}
}