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
}

void Animator::ProcessScaleAnimation(AnimationComponent& animComp, Transform& transform)
{
}

void Animator::ProcessAnimationEvents(AnimationComponent& animComp, Transform& transform)
{
}

void Animator::ProcessAnimations(AnimationComponent& animComp, Transform& transform)
{
	// ======= Process and Update the Position Animations ============
	ProcessPositionAnimation(animComp, transform);

	// ======= Process and Update the Rotaion Animations ============
	
	// ======= Process and Update the Scale Animations ============

	// ======= Process and Update the Animation Events ============
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

		LOG_ERROR("TIme : {0}", animComponent.time);

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
