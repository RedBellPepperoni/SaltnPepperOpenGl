#pragma once
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	class Plane
	{
	public:
        Plane();
        Plane(const Vector3& normal, float distance);
        Plane(const Vector3& point, const Vector3& normal);
        Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3);
        Plane(const Vector4& plane);
        Plane(float normX, float normY, float normZ, float distance);
        ~Plane() = default;

        void Set(const Vector3& normal, float distance);
        void Set(const Vector3& point, const Vector3& normal);
        void Set(const Vector3& point1, const Vector3& point2, const Vector3& point3);
        void Set(const Vector4& plane);
        void SetNormal(const Vector3& normal);
        void SetDistance(float distance);
        void Transform(const Matrix4& transform);
        Plane Transformed(const Matrix4& transform) const;

        void Normalise();

        float Distance(const Vector3& point) const;
        float Distance(const Vector4& point) const;

        bool IsPointOnPlane(const Vector3& point) const;
        bool IsPointOnPlane(const Vector4& point) const;

        Vector3 Project(const Vector3& point) const { return point - Distance(point) * m_normal; }

        inline Vector3 Normal() const { return m_normal; }
        inline float Distance() const { return m_distance; }

    private:
        Vector3 m_normal;
        float m_distance;
	};


}

