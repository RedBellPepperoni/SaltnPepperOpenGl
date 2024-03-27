#ifndef BOUNDINGCAPSULE_H
#define BOUNDINGCAPSULE_H

#include "BoundingBox.h"
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	class BoundingCapsule
	{
	public:

		enum class Axis
		{
			X, 
			Y,
			Z
		};

		float height = 0.0f;
		float radius = 0.0f;
        Vector3 center = Vector3{ 0.0f };
		Quaternion rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		Axis orientation = Axis::Y;

		constexpr BoundingCapsule() = default;

		constexpr BoundingCapsule(float Height, float Radius, Axis Orientation)
			: height(Height), radius(Radius), orientation(Orientation)
		{
		}
	};

    constexpr BoundingCapsule ToCapsule(const BoundingBox& aabb, BoundingCapsule::Axis axis)
    {
        auto length = aabb.Length();
        float height = 0.0f, radius = 0.0f;
        switch (axis)
        {
        case BoundingCapsule::Axis::X:
            radius = (length.y + length.z) * 0.25f;
            height = Max(0.0f, length.x - 2.0f * radius);
            break;
        case BoundingCapsule::Axis::Y:
            radius = (length.x + length.z) * 0.25f;
            height = Max(0.0f, length.y - 2.0f * radius);
            break;
        case BoundingCapsule::Axis::Z:
            radius = (length.x + length.y) * 0.25f;
            height = Max(0.0f, length.z - 2.0f * radius);
            break;
        }
        return BoundingCapsule(height, radius, axis);
    }

    inline constexpr bool operator==(const BoundingCapsule& b1, const BoundingCapsule& b2)
    {
        return b1.center == b2.center && b1.height == b2.height && b1.orientation == b2.orientation && b1.radius == b2.radius && b1.rotation == b2.rotation;
    }

    inline constexpr bool operator!=(const BoundingCapsule& b1, const BoundingCapsule& b2)
    {
        return !(b1 == b2);
    }
	
}

#endif // !BOUNDINGCAPSULE_H
