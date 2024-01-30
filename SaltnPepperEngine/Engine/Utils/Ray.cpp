#include "Ray.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"

namespace SaltnPepperEngine
{
	Ray::Ray()
	{
		m_origin = Vector3{ 0.0f };
		m_direction = Vector3{ 0.0f };
	}

	Ray::Ray(const Vector3& Origin, const Vector3& Direction)
	{
		m_origin = Origin;
		m_direction = Direction;
	}

	bool Ray::CheckIntersection(const BoundingBox& Box) const
	{
		float distance = 0.0f;
		return CheckIntersection(Box, distance);
	}

	bool Ray::CheckIntersection(const BoundingBox& Box, float& t) const
	{
		t = 0.0f;

		if (Box.CheckIntersection(m_origin))
		{
			return true;
		}

		// Just a huge value for now
		float distance = FLT_MAX;

        // Checking the intersection in the X - Direction
		if (m_origin.x < Box.Min().x && m_direction.x > 0.0f)
		{
			float x = (Box.Min().x - m_origin.x) / m_direction.x;

			if (x < distance)
			{
				Vector3 point = m_origin + x * m_direction;

				if (point.y >= Box.Min().y && point.y <= Box.Max().y && point.z >= Box.Min().z && point.z <= Box.Max().z)
				{
					distance = x;
				}
			}
		}

        if (m_origin.x > Box.Max().x && m_direction.x < 0.0f)
        {
            float x = (Box.Max().x - m_origin.x) / m_direction.x;
            if (x < distance)
            {
                Vector3 point = m_origin + x * m_direction;
                if (point.y >= Box.Min().y && point.y <= Box.Max().y && point.z >= Box.Min().z && point.z <= Box.Max().z)
                    distance = x;
            }
        }

        // Checking for Intersection in the Y - Direction
        if (m_origin.y < Box.Min().y && m_direction.y > 0.0f)
        {
            float x = (Box.Min().y - m_origin.y) / m_direction.y;
            if (x < distance)
            {
                Vector3 point = m_origin + x * m_direction;
                if (point.x >= Box.Min().x && point.x <= Box.Max().x && point.z >= Box.Min().z && point.z <= Box.Max().z)
                    distance = x;
            }
        }


        if (m_origin.y > Box.Max().y && m_direction.y < 0.0f)
        {
            
            float x = (Box.Max().y - m_origin.y) / m_direction.y;
            if (x < distance)
            {
                Vector3 point = m_origin + x * m_direction;
                if (point.x >= Box.Min().x && point.x <= Box.Max().x && point.z >= Box.Min().z && point.z <= Box.Max().z)
                    distance = x;
            }
        }
        // Checking for intersection in the Z - Direction
        if (m_origin.z < Box.Min().z && m_direction.z > 0.0f)
        {
            float x = (Box.Min().z - m_origin.z) / m_direction.z;
            if (x < distance)
            {
                Vector3 point = m_origin + x * m_direction;
                if (point.x >= Box.Min().x && point.x <= Box.Max().x && point.y >= Box.Min().y && point.y <= Box.Max().y)
                    distance = x;
            }
        }
        if (m_origin.z > Box.Max().z && m_direction.z < 0.0f)
        {
            float x = (Box.Max().z - m_origin.z) / m_direction.z;
            if (x < distance)
            {
                Vector3 point = m_origin + x * m_direction;
                if (point.x >= Box.Min().x && point.x <= Box.Max().x && point.y >= Box.Min().y && point.y <= Box.Max().y)
                    distance = x;
            }
        }

        t = distance;

        // Should be less than a large number
        return distance < 100000000.0f;

	}

	

}