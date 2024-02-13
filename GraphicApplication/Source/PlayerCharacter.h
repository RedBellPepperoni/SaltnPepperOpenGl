#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H


#include <Engine/Utils/Maths/MathDefinitions.h>


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
		Transform* m_leftHand = nullptr;
		Transform* m_rightHand = nullptr;
		PlayerState m_currentState = PlayerState::IDLE;

		
	};


}

#endif // !PLAYERCHARACTER_H

