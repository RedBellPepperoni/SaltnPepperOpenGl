#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class CylinderCollider : public BaseCollider
		{
			SharedPtr<CylinderShape> cylinderShape;

            void CreateNewShape(const BoundingCylinder& cylinder);
        public:
            CylinderCollider() = default;
            void Init(const BoundingCylinder& cylinder);
            void UpdateCollider(const BoundingCylinder& cylinder);

            CylinderShape* GetNativeHandle() const;

            void SetOrientation(const BoundingBox& aabb, BoundingCylinder::Axis axis);
            BoundingCylinder::Axis GetOrientation() const;

            BoundingBox GetAABB(const Transform& transform) const;
            BoundingSphere GetBoundingSphere(const Transform& transform) const;
            BoundingCylinder GetBoundingCylinder(const Transform& transform) const;
            BoundingCylinder GetBoundingCylinderInternal() const;
            void SetBoundingCylinder(BoundingCylinder cylinder);
		};

	}
}

#endif // !CYLINDERCOLLIDER_H
