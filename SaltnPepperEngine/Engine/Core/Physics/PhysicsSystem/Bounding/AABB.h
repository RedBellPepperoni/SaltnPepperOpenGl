#ifndef AABB_H
#define AABB_H

#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
    class AABB
    {
    public:
        Vector3 Min = Vector3{ 0.0f };
        Vector3 Max = Vector3{ 0.0f };

        constexpr Vector3 Length() const
        {
            return Max - Min;
        }

        constexpr Vector3 GetCenter() const
        {
            return (Max + Min) * 0.5f;
        }

	};

    inline constexpr AABB operator*(const AABB& box, const Vector3& scale)
    {
        return AABB{ box.Min * scale, box.Max * scale };
    }

    inline constexpr AABB operator/(const AABB& box, const Vector3& scale)
    {
        return AABB{ box.Min / scale, box.Max / scale };
    }

    inline constexpr AABB operator+(const AABB& box, const Vector3& translate)
    {
        return AABB{ box.Min + translate, box.Max + translate };
    }

    inline constexpr AABB operator-(const AABB& box, const Vector3& translate)
    {
        return AABB{ box.Min - translate, box.Max - translate };
    }

    inline constexpr bool operator==(const AABB& box1, const AABB& box2)
    {
        constexpr auto abs = [](float x) { return x >= 0.0f ? x : -x; };

        auto d1 = box1.Min - box2.Min;
        auto d2 = box1.Max - box2.Max;
        bool res1 = abs(d1.x) < 0.01f && abs(d1.y) < 0.01f && abs(d1.z) < 0.01f;
        bool res2 = abs(d2.x) < 0.01f && abs(d2.y) < 0.01f && abs(d2.z) < 0.01f;
        return res1 && res2;
    }

    inline constexpr bool operator!=(const AABB& box1, const AABB& box2)
    {
        return !(box1 == box2);
    }

    inline AABB operator*(const AABB& box, const Matrix4& matrix)
    {
        std::array<Vector3, 8> vecs;
        vecs =
        {
            matrix * Vector4(box.Min.x, box.Min.y, box.Min.z, 1.0f),
            matrix * Vector4(box.Min.x, box.Min.y, box.Max.z, 1.0f),
            matrix * Vector4(box.Min.x, box.Max.y, box.Min.z, 1.0f),
            matrix * Vector4(box.Min.x, box.Max.y, box.Max.z, 1.0f),
            matrix * Vector4(box.Max.x, box.Min.y, box.Min.z, 1.0f),
            matrix * Vector4(box.Max.x, box.Min.y, box.Max.z, 1.0f),
            matrix * Vector4(box.Max.x, box.Max.y, box.Min.z, 1.0f),
            matrix * Vector4(box.Max.x, box.Max.y, box.Max.z, 1.0f),
        };

        auto minmax = GetMinMaxBounds(vecs.data(), vecs.size());
        return AABB{ minmax.first, minmax.second };
    }
}

#endif // !AABB_H
