#include "Frustum.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h"
#include "Engine/Utils/Ray.h"


namespace SaltnPepperEngine
{
	Frustum::Frustum()
	{
		Define(Matrix4(1.0f));
	}

	Frustum::Frustum(const Matrix4& transform)
	{
		Define(transform);
	}

	Frustum::Frustum(const Matrix4& projection, const Matrix4& view)
	{
		Matrix4 mat = projection * view;
		Define(mat);
	}

	Frustum::~Frustum()
	{
	}

	void Frustum::Transform(const Matrix4& transform)
	{
		for (int i = 0; i < 6; i++)
		{
			m_planes[i].Transform(transform);
		}

		for (int i = 0; i < 6; i++)
		{
			m_planes[i].Normalise();
		}

		CalculateVertices(transform);
	}

	void Frustum::Define(const Matrix4& projection, const Matrix4& view)
	{
		Matrix4 mat = projection * view;
		Define(mat);
	}

	void Frustum::Define(const Matrix4& transform)
	{
		Matrix4 mat = transform;
		m_planes[PLANE_LEFT] = Plane(mat[0][3] + mat[0][0], mat[1][3] + mat[1][0], mat[2][3] + mat[2][0], mat[3][3] + mat[3][0]);
		m_planes[PLANE_RIGHT] = Plane(mat[0][3] - mat[0][0], mat[1][3] - mat[1][0], mat[2][3] - mat[2][0], mat[3][3] - mat[3][0]);
		m_planes[PLANE_DOWN] = Plane(mat[0][3] + mat[0][1], mat[1][3] + mat[1][1], mat[2][3] + mat[2][1], mat[3][3] + mat[3][1]);
		m_planes[PLANE_UP] = Plane(mat[0][3] - mat[0][1], mat[1][3] - mat[1][1], mat[2][3] - mat[2][1], mat[3][3] - mat[3][1]);
		m_planes[PLANE_NEAR] = Plane(mat[0][3] + mat[0][2], mat[1][3] + mat[1][2], mat[2][3] + mat[2][2], mat[3][3] + mat[3][2]);
		m_planes[PLANE_FAR] = Plane(mat[0][3] - mat[0][2], mat[1][3] - mat[1][2], mat[2][3] - mat[2][2], mat[3][3] - mat[3][2]);

		for (int i = 0; i < 6; i++)
		{
			m_planes[i].Normalise();
		}

		CalculateVertices(transform);
	}

	void Frustum::DefineOrtho(float scale, float aspectRatio, float n, float f, const Matrix4& viewMatrix)
	{
		Matrix4 mat = glm::ortho(-scale * aspectRatio, scale * aspectRatio, -scale, scale, n, f);
		mat = mat * viewMatrix;
		Define(mat);
	}

	void Frustum::Define(float fov, float aspectRatio, float n, float f, const Matrix4& viewMatrix)
	{
		float tangent = tan(fov * 0.5f);
		float height = n * tangent;
		float width = height * aspectRatio;

		Matrix4 mat = glm::frustum(-width, width, -height, height, n, f);
		mat = mat * viewMatrix;
		Define(mat);
	}

	bool Frustum::Intersects(const Vector3& point) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_planes[i].Distance(point) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool Frustum::Intersects(const BoundingSphere& sphere) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_planes[i].Distance(sphere.GetCenter()) < -sphere.GetRadius())
			{
				return false;
			}
		}

		return true;
	}

	bool Frustum::Intersects(const BoundingBox& box) const
	{
		for (int i = 0; i < 6; i++)
		{
			Vector3 p = box.Min(), n = box.Max();
			Vector3 N = m_planes[i].Normal();
			if (N.x >= 0)
			{
				p.x = box.Max().x;
				n.x = box.Min().x;
			}
			if (N.y >= 0)
			{
				p.y = box.Max().y;
				n.y = box.Min().y;
			}
			if (N.z >= 0)
			{
				p.z = box.Max().z;
				n.z = box.Min().z;
			}

			if (m_planes[i].Distance(p) < 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Frustum::Intersects(const Ray& ray) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_planes[i].Distance(ray.m_origin) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	bool Frustum::Intersects(const Plane& plane) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (m_planes[i].Distance(plane.Normal()) < 0.0f)
			{
				return false;
			}
		}

		return true;
	}

	const Plane& Frustum::GetPlane(FrustumPlane plane) const
	{
		return m_planes[plane];
	}

	const Plane& Frustum::GetPlane(int index) const
	{
		return m_planes[index];
	}
	Vector3* Frustum::GetVerticies()
	{
		return m_vertices;
	}
	void Frustum::CalculateVertices(const Matrix4& transform)
	{
		static const bool zerotoOne = false;
		static const bool leftHand = true;

		Matrix4 transformInv = Inverse(transform);

		m_vertices[0] = Vector4(-1.0f, -1.0f, zerotoOne ? 0.0f : -1.0f, 1.0f);
		m_vertices[1] = Vector4(1.0f, -1.0f, zerotoOne ? 0.0f : -1.0f, 1.0f);
		m_vertices[2] = Vector4(1.0f, 1.0f, zerotoOne ? 0.0f : -1.0f, 1.0f);
		m_vertices[3] = Vector4(-1.0f, 1.0f, zerotoOne ? 0.0f : -1.0f, 1.0f);

		m_vertices[4] = Vector4(-1.0f, -1.0f, 1.0f, 1.0f);
		m_vertices[5] = Vector4(1.0f, -1.0f, 1.0f, 1.0f);
		m_vertices[6] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertices[7] = Vector4(-1.0f, 1.0f, 1.0f, 1.0f);

		Vector4 temp;
		for (int i = 0; i < 8; i++)
		{
			temp = transformInv * Vector4(m_vertices[i], 1.0f);
			m_vertices[i] = temp / temp.w;
		}
	}
}
