#ifndef SECURITYCAMERA_H
#define SECURITYCAMERA_H
#include "Engine/Core/Components/Transform.h"

namespace SaltnPepperEngine
{



	class SecurityCamera
	{
	public:
		void Update(Transform& transform, const float& deltaTime)
		{
			float yawRotation = transform.GetEulerRotation().y;

			if (sweepRight)
			{
				yawRotation += rotationSpeed * rotationMultiplier * deltaTime;
			}
			else
			{
				yawRotation -= rotationSpeed * rotationMultiplier * deltaTime;
			}

			if (yawRotation < minAngle )
			{
				yawRotation = minAngle;
				sweepRight = true;
			}

			else if (yawRotation > maxAngle)
			{
				yawRotation = maxAngle;
				sweepRight = false;
			}

			transform.SetEularRotation(Vector3(0.0f, yawRotation, 0.0f));
		}

		

	private:
		bool sweepRight = false;
		float rotationSpeed = 1.0f;
		float rotationMultiplier = 10.0f;
		
		float minAngle = -40.0f;
		float maxAngle = 40.0f;

		

	};


}

#endif // !SECURITYCAMERA_H

