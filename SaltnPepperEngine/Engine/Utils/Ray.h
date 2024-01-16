#ifndef RAY_H
#define RAY_H
#include "Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	
	class BoundingBox;
	

	

	class Ray
	{
	public:

		Ray();
		Ray(const Vector3& Origin, const Vector3& Direction);


		bool CheckIntersection(const BoundingBox& Box) const;
		bool CheckIntersection(const BoundingBox& Box, float& t) const;
		
		Vector3 m_origin = Vector3{0.0f};
		Vector3 m_direction = Vector3{ 0.0f };

	};
}


#endif // !RAY_H

