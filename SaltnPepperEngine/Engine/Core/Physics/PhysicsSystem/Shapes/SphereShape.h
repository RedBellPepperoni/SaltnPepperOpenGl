#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "BaseShape.h"

class btSphereShape;

namespace SaltnPepperEngine
{

	namespace Physics
	{
		class SphereShape : public BaseShape
		{
		public:

			using NativeHandle = btSphereShape*;

			SphereShape(float radius);
			SphereShape(const BoundingSphere& sphere);
			SphereShape(const SphereShape& ) = delete;
			SphereShape(SphereShape&& other) noexcept;
			SphereShape& operator=(const SphereShape&) = delete;
			SphereShape& operator=(SphereShape&& other) noexcept;
			~SphereShape();

			// other functions defined in ShapeBase

			BoundingSphere GetNativeBounding() const;
			BoundingSphere GetNativeBoundingTransformed(const Transform& transform) const;

		};
	}

}

#endif // !SPHERESHAPE_H
