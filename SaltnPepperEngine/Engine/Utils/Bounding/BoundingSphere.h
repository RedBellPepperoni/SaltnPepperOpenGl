#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	class BoundingSphere
	{
	public:

		BoundingSphere();
		BoundingSphere(const Vector3& center, const float radius);
		BoundingSphere(const BoundingSphere& otherSphere);

		~BoundingSphere();


		void Transform(const Matrix4& newtransform);

		void SetCenter(const Vector3& newCenter);
		void SetRadius(const float newRadius);


		bool IsInside(const BoundingSphere& otherSphere) const;
		bool Intersects(const BoundingSphere& otherSphere) const;



		const float GetRadius() const;
		const Vector3& GetCenter() const;

	private:

		Vector3 m_center;
		float m_radius;


	};
}

#endif // !BOUNDINGSPHERE_H
