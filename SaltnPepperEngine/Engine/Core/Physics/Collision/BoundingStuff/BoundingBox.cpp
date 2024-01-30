#include "BoundingBox.h"

namespace SaltnPepperEngine
{
	BoundingBox::BoundingBox()
	{
		m_min = Vector3(FLT_MAX);
		m_max = Vector3(-FLT_MAX);
	}

	BoundingBox::BoundingBox(const Vector3& min, const Vector3& max)
	{
		m_min = min;
		m_max = max;
	}
	BoundingBox::BoundingBox(const BoundingBox& otherBox)
	{
		m_min = otherBox.m_min;
		m_max = otherBox.m_max;
	}

	BoundingBox::BoundingBox(BoundingBox&& otherBox)
	{
		m_min = otherBox.m_min;
		m_max = otherBox.m_max;
	}

	BoundingBox::~BoundingBox()
	{

	}

	void BoundingBox::Reset()
	{
		m_min = Vector3(FLT_MAX);
		m_max = Vector3(-FLT_MAX);
	}

	void BoundingBox::Set(const Vector3& min, const Vector3& max)
	{
		m_min = min;
		m_max = 
			
			max;
	}

	BoundingBox& BoundingBox::operator=(const BoundingBox& other)
	{
		m_min = other.m_min;
		m_max = other.m_max;

		return *this;
	}

	BoundingBox& BoundingBox::operator=(BoundingBox&& other)
	{
		m_min = other.m_min;
		m_max = other.m_max;

		return *this;
	}

	void BoundingBox::Translate(const Vector3& translation)
	{
		m_min += translation;
		m_max += translation;
	}
	void BoundingBox::Scale(const Vector3& scale)
	{
		m_min *= scale;
		m_max *= scale;
	}

	void BoundingBox::Rotate(const Matrix3& rotation)
	{
		Vector3 RotatedExtents = Vector3(rotation * Vector4(GetExtents(), 1.0f));

		m_min = Center() - RotatedExtents;
		m_max = Center() + RotatedExtents;
	}

	void BoundingBox::Transform(const Matrix4& transform)
	{

		Vector3 newCenter = transform * Vector4(Center(), 1.0f);
		Vector3 oldEdge = Size() * 0.5f;
		Vector3 NewEdge = Vector3(
			Abs(transform[0][0]) * oldEdge.x + Abs(transform[1][0]) * oldEdge.y + Abs(transform[2][0]) * oldEdge.z,
			Abs(transform[0][1]) * oldEdge.x + Abs(transform[1][1]) * oldEdge.y + Abs(transform[2][1]) * oldEdge.z,
			Abs(transform[0][2]) * oldEdge.x + Abs(transform[1][2]) * oldEdge.y + Abs(transform[2][2]) * oldEdge.z);


		m_min = newCenter - NewEdge;
		m_max = newCenter + NewEdge;

	}
	BoundingBox BoundingBox::GetTransformedBox(const Matrix4& transform)
	{
		//Making a copy of this Box
		BoundingBox newBox(*this);

		// Transforming teh copied box
		newBox.Transform(transform);

		return newBox;

	}
	IntersectionResult BoundingBox::CheckInterscetion(const BoundingBox& otherBox) const
	{
		if (otherBox.m_max.x < m_min.x || otherBox.m_min.x > m_max.x || otherBox.m_max.y < m_min.y || otherBox.m_min.y > m_max.y || otherBox.m_max.z < m_min.z || otherBox.m_min.z > m_max.z)
			return OUTSIDE;
		else if (otherBox.m_min.x < m_min.x || otherBox.m_max.x > m_max.x || otherBox.m_min.y < m_min.y || otherBox.m_max.y > m_max.y || otherBox.m_min.z < m_min.z || otherBox.m_max.z > m_max.z)
			return INTERSECTING;
		else
			return INSIDE;
	}

	// Implement his later incase needed
	IntersectionResult BoundingBox::CheckInterscetion(const BoundingSphere& otherSphere) const
	{
		return IntersectionResult();
	}

	IntersectionResult BoundingBox::CheckIntersection(const Vector3& point) const
	{
		if (point.x < m_min.x || point.x > m_max.x || point.y < m_min.y || point.y > m_max.y || point.z < m_min.z || point.z > m_max.z)
		{
			return OUTSIDE;
		}
		else
		{
			return INSIDE;
		}
	}

	void BoundingBox::Merge(const Vector3& point)
	{
		if (point.x < m_min.x)
		{
			m_min.x = point.x;
		}

		if (point.y < m_min.y)
		{
			m_min.y = point.y;
		}

		if (point.z < m_min.z)
		{
			m_min.z = point.z;
		}

		if (point.x > m_max.x)
		{
			m_max.x = point.x;
		}

		if (point.y > m_max.y)
		{
			m_max.y = point.y;
		}

		if (point.z > m_max.z)
		{
			m_max.z = point.z;
		}
				
		
	}

	void BoundingBox::Merge(const BoundingBox& other)
	{
		if (other.m_min.x < m_min.x)
		{
			m_min.x = other.m_min.x;
		}
               
		if (other.m_min.y < m_min.y)
		{
			m_min.y = other.m_min.y;
		}
               
		if (other.m_min.z < m_min.z)
		{
			m_min.z = other.m_min.z;
		}
               
		if (other.m_max.x > m_max.x)
		{
			m_max.x = other.m_max.x;
		}
                
		if (other.m_max.y > m_max.y)
		{
			m_max.y = other.m_max.y;
		}
                
		if (other.m_max.z > m_max.z)
		{
			m_max.z = other.m_max.z;
		}
               
	}

	Vector3 BoundingBox::Size() const
	{
		return m_max - m_min;
	}
	Vector3 BoundingBox::Center() const
	{
		return ((m_min + m_max) * 0.5f);
	}
	Vector3 BoundingBox::Min() const
	{
		return m_min;
	}
	Vector3 BoundingBox::Max() const
	{
		return m_max;
	}
	Vector3 BoundingBox::GetExtents() const
	{
		return m_max - m_min;
	}
}
