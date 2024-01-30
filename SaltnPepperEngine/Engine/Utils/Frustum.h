#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Plane.h"

namespace SaltnPepperEngine
{


    class Ray;
    class BoundingBox;
    class BoundingSphere;

    enum FrustumPlane
    {
        PLANE_NEAR = 0,
        PLANE_LEFT,
        PLANE_RIGHT,
        PLANE_UP,
        PLANE_DOWN,
        PLANE_FAR,
    };

	class Frustum
	{
	public:

        Frustum();
        Frustum(const Matrix4& transform);
        Frustum(const Matrix4& projection, const Matrix4& view);
        ~Frustum();

        void Transform(const Matrix4& transform);
        void Define(const Matrix4& projection, const Matrix4& view);
        void Define(const Matrix4& transform);
        void DefineOrtho(float scale, float aspectRatio, float n, float f, const Matrix4& viewMatrix);
        void Define(float fov, float aspectRatio, float n, float f, const Matrix4& viewMatrix);

        bool Intersects(const Vector3& point) const;
        bool Intersects(const BoundingSphere& sphere) const;
        bool Intersects(const BoundingBox& box) const;
        bool Intersects(const Ray& ray) const;
        bool Intersects(const Plane& plane) const;

        const Plane& GetPlane(FrustumPlane plane) const;
        const Plane& GetPlane(int index) const;

        Vector3* GetVerticies();

    private:
        void CalculateVertices(const Matrix4& transform);

        Plane m_planes[6];
        Vector3 m_vertices[8];

	};

	
}


#endif // !FRUSTUM_H


