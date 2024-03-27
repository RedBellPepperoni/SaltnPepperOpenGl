#ifndef CYLINDERSHAPE_H
#define CYLINDERSHAPE_H

#include "BaseShape.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingCylinder.h"

class btCylinderShape;

namespace SaltnPepperEngine
{

	namespace Physics
	{
        class CylinderShape : public BaseShape
        {
            BoundingCylinder::Axis orientation = BoundingCylinder::Axis::Y;
        public:
            using NativeHandle = btCylinderShape*;

            CylinderShape(const BoundingCylinder& cylinder);
            CylinderShape(const CylinderShape&) = delete;
            CylinderShape(CylinderShape&& other) noexcept;
            CylinderShape& operator=(const CylinderShape&) = delete;
            CylinderShape& operator=(CylinderShape&& other) noexcept;
            ~CylinderShape();

            BoundingCylinder GetBoundingCylinderTransformed(const Transform& transform) const;
            BoundingCylinder GetBoundingCylinder() const;
            BoundingCylinder GetNativeBoundingTransformed(const Transform& transform) const;
            BoundingCylinder GetNativeBounding() const;
            BoundingCylinder::Axis GetOrientation() const;
        };
	}
}
#endif // !CYLINDERSHAPE_H
