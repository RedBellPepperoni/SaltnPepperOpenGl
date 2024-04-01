#ifndef CAPSULESHAPE_H
#define CAPSULESHAPE_H
#include "BaseShape.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingCapsule.h"


class btCapsuleShape;

namespace SaltnPepperEngine
{

	namespace Physics
	{
		class CapsuleShape : public BaseShape
		{
			BoundingCapsule::Axis orientation = BoundingCapsule::Axis::Y;
		public:

            using NativeHandle = btCapsuleShape*;

            CapsuleShape(const BoundingCapsule& capsule);
            CapsuleShape(const CapsuleShape&) = delete;
            CapsuleShape(CapsuleShape&& other) noexcept;
            CapsuleShape& operator=(const CapsuleShape&) = delete;
            CapsuleShape& operator=(CapsuleShape&& other) noexcept;
            ~CapsuleShape();

            BoundingCapsule GetBoundingCapsuleTransformed(const Transform& transform) const;
            BoundingCapsule GetBoundingCapsule() const;
            BoundingCapsule GetNativeBounding() const;
            BoundingCapsule GetNativeBoundingTransformed(const Transform& transform) const;
            BoundingCapsule::Axis GetOrientation() const;
		};


	}
}

#endif // !CAPSULESHAPE_H

