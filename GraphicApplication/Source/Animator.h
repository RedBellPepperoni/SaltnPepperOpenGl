#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Application/Application.h"
#include "AnimationDefinitions.h"

namespace SaltnPepperEngine
{
	class EntityManager;

	
}

using namespace SaltnPepperEngine;
using namespace Components;

class Animator
{
private: 


	void ProcessPositionAnimation(AnimationComponent& animComp, Transform& transform);
	void ProcessRotationAnimation(AnimationComponent& animComp, Transform& transform);
	void ProcessScaleAnimation(AnimationComponent& animComp, Transform& transform);
	void ProcessAnimationEvents(AnimationComponent& animComp, Transform& transform);
	void ProcessAnimations(AnimationComponent& animComp, Transform& transform);
	


public:
	Animator() = default;

	Animator(EntityManager* enttmanager)
	{
		Init(enttmanager);
	}

	~Animator() = default;

	void Init(EntityManager* enttmanager);
	void Update(const float deltaTime);

	void SetPlayback(bool reverse = false);
	void SetAnimSpeed(float Speed);

private:

	EntityManager* manager = nullptr;
	bool reversePlayback = false;

	float animSpeed = 1.0f;


};


#endif // !ANIMATOR_H
