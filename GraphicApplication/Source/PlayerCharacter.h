#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H


#include <Engine/Utils/Maths/MathDefinitions.h>

struct AnimationComponent;

namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	namespace Physics
	{
		class RigidBody3D;
	}

	using namespace Physics;
	using namespace Components;


	

	enum class PlayerState : uint8_t
	{
		IDLE,
		WALKING,
		RUNNING
	};

	class PlayerCharacter
	{
	public:

		PlayerCharacter();
		~PlayerCharacter();

		void Init(RigidBody3D* RigidBodyRef, Transform* cameraTransformRef);
		void SetAnimators(AnimationComponent* left, AnimationComponent* right);
		void Update(float deltaTime);
		void ProcessKeyboardInput(Transform& cameratransform, float deltaTime);

	private:

		void AnimateHands();
		void ProcessMouseInput();

	private:

		RigidBody3D* m_rigidBodyRef = nullptr;
		
		Transform* m_cameraRef = nullptr;
		int m_health = 100;
		float m_moveSpeed = 2.0f;
		float m_jumpSpeed = 25.0f;
		float m_SprintMultiplier = 2.0f;

		PlayerState m_currentState = PlayerState::IDLE;

		AnimationComponent* leftHandAnimator = nullptr;
		AnimationComponent* rightHandAnimator = nullptr;
		bool canJump = false;
		
	};


}

#endif // !PLAYERCHARACTER_H

