#ifndef INTERPOLATION_H
#define INTERPOLATION_H
#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{

	struct KeyPosition
	{
		Vector3 position;
		float timeStamp;
	};

	struct KeyRotation
	{
		Quaternion orientation;
		float timeStamp;
	};

	struct KeyScale
	{
		Vector3 scale;
		float timeStamp;
	};


	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
	

	Matrix4 interpolatePosition(float animationTime, KeyPosition from, KeyPosition to);
	

	Matrix4 interpolateRotation(float animationTime, KeyRotation from, KeyRotation to);
	

	Matrix4 interpolateScaling(float animationTime, KeyScale from, KeyScale to);

}

#endif // !INTERPOLATION_H

