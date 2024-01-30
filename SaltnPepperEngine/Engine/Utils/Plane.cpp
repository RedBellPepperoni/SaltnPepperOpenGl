#include "Plane.h"

namespace SaltnPepperEngine
{
	Plane::Plane()
	{
		m_normal = Vector3(0.0f, 1.0f, 0.0f);
		m_distance = 0.0f;
	}

	Plane::Plane(const Vector3& normal, float distance)
	{
		m_normal = Normalize(normal);
		m_distance = distance;
	}

	Plane::Plane(const Vector3& point, const Vector3& normal)
	{
		m_normal = Normalize(normal);
		m_distance = Dot(normal,point);
	}

	Plane::Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3)
	{
		Vector3 edge1 = point2 - point1;
		Vector3 edge2 = point3 - point1;
		m_normal = Normalize(Cross(edge1, edge2));
		m_distance = Dot(m_normal, point1);
	}


	Plane::Plane(const Vector4& plane)
	{
		m_normal = Vector3(plane.x, plane.y, plane.z);
		m_distance = plane.w;
	}

	Plane::Plane(float normX, float normY, float normZ, float distance)
	{
		m_normal = Vector3(normX, normY, normZ);
		m_distance = distance;

	}
	

	void Plane::Set(const Vector3& normal, float distance)
	{
		m_normal = normal;
		m_distance = distance;
	}

	void Plane::Set(const Vector3& point, const Vector3& normal)
	{
		m_normal = Normalize(normal);
		m_distance = Dot(normal, point);
	}


	void Plane::Set(const Vector3& point1, const Vector3& point2, const Vector3& point3)
	{
		Vector3 edge1 = point2 - point1;
		Vector3 edge2 = point3 - point1;
		m_normal = Normalize(Cross(edge1, edge2));
		m_distance = Dot(m_normal, point1);
	}

	void Plane::Set(const Vector4& plane)
	{
		m_normal = Vector3(plane.x, plane.y, plane.z);
		m_distance = plane.w;
	}

	void Plane::SetNormal(const Vector3& normal)
	{
		m_normal = normal;
	}


	void Plane::SetDistance(float distance)
	{
		m_distance = distance;
	}

	void Plane::Transform(const Matrix4& transform)
	{
		Vector4 plane = Vector4(m_normal, m_distance);
		plane = transform * plane;
		m_normal = Vector3(plane.x, plane.y, plane.z);
		m_distance = plane.w;

	}

	Plane Plane::Transformed(const Matrix4& transform) const
	{
		Vector4 plane = Vector4(m_normal, m_distance);
		plane = transform * plane;

		return Plane(Vector3(plane.x,plane.y,plane.z), plane.w);
	}

	void Plane::Normalise()
	{
		float magnitude = Length(m_normal);
		m_normal /= magnitude;

		m_distance /= magnitude;

	}

	float Plane::Distance(const Vector3& point) const
	{
		return Dot(point, m_normal) + m_distance;
	}

	float Plane::Distance(const Vector4& point) const
	{
		return Dot(Vector3(point), m_normal) + m_distance;
	}

	bool Plane::IsPointOnPlane(const Vector3& point) const
	{
		return Distance(point) >= -0.0001f;
	}

	bool Plane::IsPointOnPlane(const Vector4& point) const
	{
		return Distance(point) >= -0.00001f;
	}

}