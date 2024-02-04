#include "Animator.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include <glm/gtx/easing.hpp>
#include <Engine/Utils/Logging/Log.h>


using namespace SaltnPepperEngine;


void Animator::Init(EntityManager* enttmanager)
{
	manager = enttmanager;
}

void Animator::ProcessPositionAnimation(AnimationComponent& animComp, Transform& transform)
{
	size_t positionListSize = animComp.positionKeyFrameList.size();

	// early check to see if there are any positions at all
	if (positionListSize < 1)
	{
		// No position keyframe data
		return;
	}

	

	// Edge case where thers only one keyframe
	if (positionListSize == 1)
	{

		Vector3 position = animComp.positionKeyFrameList[0].keyData;
		transform.SetPosition(position);

		return;
	}
	

	size_t keyframeEndIndex = 0;

	for (const KeyFrame& positionKey : animComp.positionKeyFrameList)
	{
		if (positionKey.time > animComp.time)
		{
			break;
		}

		keyframeEndIndex++;
	}

	size_t keyframeStartIndex = keyframeEndIndex - 1;

	if (keyframeEndIndex >= positionListSize)
	{
		transform.SetPosition(animComp.positionKeyFrameList[keyframeStartIndex].keyData);
		return;
	}

	KeyFrame startPosKey = animComp.positionKeyFrameList[keyframeStartIndex];
	KeyFrame endPosKey = animComp.positionKeyFrameList[keyframeEndIndex];


	float percent = (animComp.time - startPosKey.time) / (endPosKey.time - startPosKey.time);
	float result = 0.0f;

	switch (endPosKey.easeType)
	{
	case EasingType::Linear: result = percent;
		break;
	case EasingType::SineEaseIn: result = glm::sineEaseIn(percent);
		break;
	case EasingType::SineEaseOut: result = glm::sineEaseOut(percent);
		break;
	case EasingType::SineEaseInOut: result = glm::sineEaseInOut(percent);
		break;
	default: result = percent;
		break;
	}

	Vector3 delta = endPosKey.keyData - startPosKey.keyData;
	transform.SetPosition(startPosKey.keyData + delta * result);

}

void Animator::ProcessRotationAnimation(AnimationComponent& animComp, Transform& transform)
{
	size_t rotationListSize = animComp.rotationKeyFrameList.size();

	// early check to see if there are any positions at all
	if (rotationListSize < 1)
	{
		// No position keyframe data
		return;
	}



	// Edge case where thers only one keyframe
	if (rotationListSize == 1)
	{

		Vector3 rotation = animComp.rotationKeyFrameList[0].keyData;
		transform.SetEularRotation(rotation);

		return;
	}


	size_t keyframeEndIndex = 0;

	for (const KeyFrame& rotationKey : animComp.rotationKeyFrameList)
	{
		if (rotationKey.time > animComp.time)
		{
			break;
		}

		keyframeEndIndex++;
	}

	size_t keyframeStartIndex = keyframeEndIndex - 1;

	if (keyframeEndIndex >= rotationListSize)
	{
		transform.SetEularRotation(animComp.rotationKeyFrameList[keyframeStartIndex].keyData);
		return;
	}

	KeyFrame& startRotKey = animComp.rotationKeyFrameList[keyframeStartIndex];
	KeyFrame& endRotKey = animComp.rotationKeyFrameList[keyframeEndIndex];


	float percent = (animComp.time - startRotKey.time) / (endRotKey.time - startRotKey.time);
	float result = 0.0f;

	switch (endRotKey.easeType)
	{
	case EasingType::Linear: result = percent;
		break;
	case EasingType::SineEaseIn: result = glm::sineEaseIn(percent);
		break;
	case EasingType::SineEaseOut: result = glm::sineEaseOut(percent);
		break;
	case EasingType::SineEaseInOut: result = glm::sineEaseInOut(percent);
		break;
	default: result = percent;
		break;
	}

	Quaternion startQuat = Quaternion(Vector3(startRotKey.keyData.x * DEGtoRAD, startRotKey.keyData.y * DEGtoRAD, startRotKey.keyData.z * DEGtoRAD));
	Quaternion endQuat = Quaternion(Vector3(endRotKey.keyData.x * DEGtoRAD, endRotKey.keyData.y * DEGtoRAD, endRotKey.keyData.z * DEGtoRAD));

	transform.SetRotation(glm::slerp(startQuat, endQuat, result));

}

void Animator::ProcessScaleAnimation(AnimationComponent& animComp, Transform& transform)
{

	size_t scaleListSize = animComp.scaleKeyFrameList.size();

	// early check to see if there are any positions at all
	if (scaleListSize < 1)
	{
		// No position keyframe data
		return;
	}



	// Edge case where thers only one keyframe
	if (scaleListSize == 1)
	{

		Vector3 position = animComp.scaleKeyFrameList[0].keyData;
		transform.SetScale(position);

		return;
	}


	size_t keyframeEndIndex = 0;

	for (const KeyFrame& positionKey : animComp.scaleKeyFrameList)
	{
		if (positionKey.time > animComp.time)
		{
			break;
		}

		keyframeEndIndex++;
	}

	size_t keyframeStartIndex = keyframeEndIndex - 1;

	if (keyframeEndIndex >= scaleListSize)
	{
		transform.SetScale(animComp.scaleKeyFrameList[keyframeStartIndex].keyData);
		return;
	}

	KeyFrame startScaleKey = animComp.scaleKeyFrameList[keyframeStartIndex];
	KeyFrame endScaleKey = animComp.scaleKeyFrameList[keyframeEndIndex];


	float percent = (animComp.time - startScaleKey.time) / (endScaleKey.time - startScaleKey.time);
	float result = 0.0f;

	switch (endScaleKey.easeType)
	{
	case EasingType::Linear: result = percent;
		break;
	case EasingType::SineEaseIn: result = glm::sineEaseIn(percent);
		break;
	case EasingType::SineEaseOut: result = glm::sineEaseOut(percent);
		break;
	case EasingType::SineEaseInOut: result = glm::sineEaseInOut(percent);
		break;
	default: result = percent;
		break;
	}

	Vector3 delta = endScaleKey.keyData - startScaleKey.keyData;
	transform.SetScale(startScaleKey.keyData + delta * result);
}

void Animator::ProcessAnimationEvents(AnimationComponent& animComp, Transform& transform)
{
	size_t eventListSize = animComp.eventKeyList.size();

	// early check to see if there are any positions at all
	if (eventListSize < 1)
	{
		// No position keyframe data
		return;
	}



	// Edge case where thers only one keyframe
	if (eventListSize == 1)
	{

		animComp.eventKeyList[0].animeventCallback();
		
		return;
	}


	size_t keyframeEndIndex = 0;

	for (const EventKey& eventKey : animComp.eventKeyList)
	{
		if (eventKey.time > animComp.time)
		{
			break;
		}

		keyframeEndIndex++;
	}

	
}

void Animator::ProcessAnimations(AnimationComponent& animComp, Transform& transform)
{
	// ======= Process and Update the Position Animations ============
	ProcessPositionAnimation(animComp, transform);

	// ======= Process and Update the Rotaion Animations ============
	ProcessRotationAnimation(animComp, transform);
	
	// ======= Process and Update the Scale Animations ============
	ProcessScaleAnimation(animComp, transform);

	// ======= Process and Update the Animation Events ============
	//ProcessAnimationEvents(animComp, transform);

}

void Animator::Update(const float deltaTime)
{
	float elapsedTime = Application::GetCurrent().GetDelta();

	// get a view of all the entites with the animation component
	ComponentView animationView = manager->GetComponentsOfType<AnimationComponent>();

	// run through all the animation components and update it
	for (Entity animationEntity : animationView)
	{
		// get refs to the transform and animation comps on the selcted entity
		Transform& transform = animationEntity.GetComponent<Transform>();
		AnimationComponent& animComponent = animationEntity.GetComponent<AnimationComponent>();




		// Update the time 
		animComponent.time += deltaTime;

		

		if (animComponent.time >= animComponent.totalAnimTime)
		{
			// If the animation is set to loop, reset the time
			if (animComponent.loop)
			{
				animComponent.time = 0.0f;
			}

			else
			{
				continue;
			}

		}

		
		ProcessAnimations(animComponent,transform);



	}
}

void Animator::SetPlayback(bool reverse)
{
	reversePlayback = reverse;
}
