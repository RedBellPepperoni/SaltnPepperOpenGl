#ifndef BOXSHAPE_H
#define BOXSHAPE_H

#include "BaseShape.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/OrientedBoundingBox.h"

class btBoxShape;

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class BoxShape : public BaseShape
		{
		public:
			using NativeHandle = btBoxShape*;

			//void CreateShape();

			BoxShape(const OrientedBoundingBox& boundingBox);
			BoxShape(const BoxShape&) = delete;
			BoxShape(BoxShape&& other) noexcept;

			BoxShape& operator=(const BoxShape&) = delete;
			BoxShape& operator=(BoxShape&& other) noexcept;

			virtual ~BoxShape();

			OrientedBoundingBox GetBoundingBoxTransformed(const Transform& transform) const;
			OrientedBoundingBox GetBoundingBox() const;
			OrientedBoundingBox GetNativeBounding() const;
			OrientedBoundingBox GetNativeBoundingTransformed(const Transform& transform) const;
		};
	}
}


#endif // !BOXSHAPE_H
