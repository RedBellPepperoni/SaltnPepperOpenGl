#ifndef BOXSHAPE_H
#define BOXSHAPE_H

#include "BaseShape.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/OrientedBoundingBox.h"
//#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"

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

			BoxShape(const BoundingBox& boundingBox);
			BoxShape(const BoxShape&) = delete;
			BoxShape(BoxShape&& other) noexcept;

			BoxShape& operator=(const BoxShape&) = delete;
			BoxShape& operator=(BoxShape&& other) noexcept;

			virtual ~BoxShape();

			BoundingBox GetBoundingBoxTransformed(Transform& transform) const;
			BoundingBox GetBoundingBox() const;
			BoundingBox GetNativeBounding() const;
			BoundingBox GetNativeBoundingTransformed(Transform& transform) const;


			BoundingBox box;
		};
	}
}


#endif // !BOXSHAPE_H
