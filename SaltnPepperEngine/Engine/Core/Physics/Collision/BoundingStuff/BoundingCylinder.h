#ifndef BOUNDINGCYLINDER_H
#define BOUNDINGCYLINDER_H

#include "BoundingBox.h"

namespace SaltnPepperEngine
{
    class BoundingCylinder
    {
    public:
        enum class Axis
        {
            X, Y, Z
        };

        float height = 0.0f;
        float radiusX = 0.0f;
        float radiusZ = 0.0f;
        Vector3 center = Vector3{ 0.0f };
        Quaternion rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
        Axis orientation = Axis::Y;

        constexpr BoundingCylinder() = default;

        constexpr BoundingCylinder(float Height, float RadiusX, float RadiusZ, Axis Orientation)
            : height(Height), radiusX(RadiusX), radiusZ(RadiusZ), orientation(Orientation) { }
    };

    constexpr BoundingCylinder ToCylinder(const BoundingBox& aabb, BoundingCylinder::Axis axis)
    {
        auto length = aabb.Length();
        float height = 0.0f, radiusX = 0.0f, radiusZ = 0.0f;
        switch (axis)
        {
        case BoundingCylinder::Axis::X:
            height = length.x, radiusX = length.y * 0.5f, radiusZ = length.z * 0.5f;
            break;
        case BoundingCylinder::Axis::Y:
            height = length.y, radiusX = length.x * 0.5f, radiusZ = length.z * 0.5f;
            break;
        case BoundingCylinder::Axis::Z:
            height = length.z, radiusX = length.x * 0.5f, radiusZ = length.y * 0.5f;
            break;
        }
        return BoundingCylinder(height, radiusX, radiusZ, axis);
    }

    inline constexpr bool operator==(const BoundingCylinder& b1, const BoundingCylinder& b2)
    {
        return b1.center == b2.center && b1.height == b2.height && b1.orientation == b2.orientation &&
            b1.radiusX == b2.radiusX && b1.radiusZ == b2.radiusZ && b1.rotation == b2.rotation;
    }

    inline constexpr bool operator!=(const BoundingCylinder& b1, const BoundingCylinder& b2)
    {
        return !(b1 == b2);
    }
}

#endif // !BOUNDINGCYLINDER_H
