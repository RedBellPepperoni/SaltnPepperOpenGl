#ifndef ORIENTEDBOUNDINGBOX_H
#define ORIENTEDBOUNDINGBOX_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "BoundingBox.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class OrientedBoundingBox
		{

		public:

			Vector3 center{0.0f};
			Vector3 min{ 0.0f };
			Vector3 max{ 0.0f };
			Quaternion rotation{ 1.0f,0.0f,0.0f,0.0f };

			OrientedBoundingBox() = default;

			OrientedBoundingBox(const Vector3& Center, const Vector3& HalfDims)
				: center(Center)
				, min(-HalfDims)
				, max (HalfDims)
			{

			}

			constexpr Vector3 Length() const
			{
				return max - min;
			}

			
		};

		inline constexpr OrientedBoundingBox ToOBB(const BoundingBox& aabb)
		{
			OrientedBoundingBox result;
			result.max = aabb.m_max;
			result.min = aabb.m_min;

			return result;

		}

		inline constexpr bool operator==(const OrientedBoundingBox& b1, const OrientedBoundingBox& b2)
		{
			return b1.center == b2.center && b1.max == b2.max && b1.min == b2.min && b1.rotation == b2.rotation;
		}

		inline constexpr bool operator!=(const OrientedBoundingBox& b1, const OrientedBoundingBox& b2)
		{
			return !(b1 == b2);
		}


	}
}

#endif // !ORIENTEDBOUNDINGBOX_H

