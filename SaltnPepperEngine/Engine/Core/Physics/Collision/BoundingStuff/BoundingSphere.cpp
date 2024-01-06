#include "BoundingSphere.h"

namespace SaltnPepperEngine
{
	BoundingSphere::BoundingSphere()
		: m_center(Vector3(0.0f))
		, m_radius(0.0f)
	{
	}
	BoundingSphere::BoundingSphere(const Vector3& center, const float radius)
		: m_center(center)
		, m_radius(radius)
	{
	}
	BoundingSphere::BoundingSphere(const BoundingSphere& otherSphere)
		: m_center(otherSphere.m_center)
		, m_radius(otherSphere.m_radius)
	{
	}

	BoundingSphere::~BoundingSphere()
	{
	}


	void BoundingSphere::Transform(const Matrix4& newtransform)
	{
		// Adjusting the center by translating it to the given transform
		Vector3 newcenter = newtransform * Vector4(m_center, 1.0f);

		// Updating the center value
		m_center = newcenter;

	}

	void BoundingSphere::SetCenter(const Vector3& newCenter)
	{
		m_center = newCenter;
	}

	void BoundingSphere::SetRadius(const float newRadius)
	{
		m_radius = newRadius;
	}

	bool BoundingSphere::IsInside(const BoundingSphere& otherSphere) const
	{
		// Faster than Calculating the square Root
		// Useless Optimisation, but its fine :(
		return LengthSquared(otherSphere.m_center - this->m_center) <= Square(this->m_radius + otherSphere.m_radius);
	}

	

	bool BoundingSphere::Intersects(const BoundingSphere& otherSphere) const
	{
		return DistanceSquared(otherSphere.m_center,this->m_center) <= Square(this->m_radius + otherSphere.m_radius);
	}
	const float BoundingSphere::GetRadius() const
	{
		return m_radius;
	}
	const Vector3& BoundingSphere::GetCenter() const
	{
		return m_center;
	}
}