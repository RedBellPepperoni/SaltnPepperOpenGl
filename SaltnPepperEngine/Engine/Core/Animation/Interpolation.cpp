#include "Interpolation.h"

namespace SaltnPepperEngine
{
	float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}
	Matrix4 interpolatePosition(float animationTime, KeyPosition from, KeyPosition to)
	{

		float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
		Vector3 finalPosition = glm::mix(from.position, to.position, scaleFactor);
		Matrix4 translation = Translate(Matrix4(1.0f), finalPosition);
		return translation;
	}

	Matrix4 interpolateRotation(float animationTime, KeyRotation from, KeyRotation to)
	{
		float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
		Quaternion finalRotation = glm::slerp(from.orientation, to.orientation, scaleFactor);
		finalRotation = Normalize(finalRotation);
		return glm::toMat4(finalRotation);
	}

	Matrix4 interpolateScaling(float animationTime, KeyScale from, KeyScale to)
	{
		float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
		Vector3 finalScale = glm::mix(from.scale, to.scale, scaleFactor);
		return Scale(Matrix4(1.0f), finalScale);
	}
}