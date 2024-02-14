#include "PlayerCharacter.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Application/Application.h"
#include "AnimationDefinitions.h"

namespace SaltnPepperEngine
{
	PlayerCharacter::PlayerCharacter()
	{

	}

	PlayerCharacter::~PlayerCharacter()
	{


	}

	void PlayerCharacter::Init(RigidBody3D* RigidBodyRef, Transform* cameraTransformRef)
	{
		m_rigidBodyRef = RigidBodyRef;
		m_cameraRef = cameraTransformRef;

		/*m_leftHand = leftHand;
		m_rightHand = rightHand;*/


	}

	void PlayerCharacter::SetAnimators(AnimationComponent* left, AnimationComponent* right)
	{
		leftHandAnimator = left;
		rightHandAnimator = right;
	}

	void PlayerCharacter::Update(float deltaTime)
	{
		//float velocityDown = m_rigidBodyRef->GetVelocity().y;


		//if (velocityDown >= -0.68 && velocityDown <= 0.68)
		//{
		//	canJump = true;
		//	//LOG_INFO("Grounded {0}", velocityDown);
		//}
		//else 
		//{
		//	canJump = false;
		//	//LOG_WARN("Jumping {0}" ,velocityDown);
		//}

		
	}

	void PlayerCharacter::ProcessKeyboardInput(Transform& cameratransform, float deltaTime)
	{

		if (Application::GetCurrent().GetEditorActive())
		{
			return;
		}

		float ForceXAxis = 0.0f;
		float ForceZAxis = 0.0f;
		float sprintMultiplier = 1.0f;
		float ForceYAxis = 0.0f;
		

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::A))
		{
			ForceXAxis = -1.0f;
		}

		// ================ LEFT CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::D))
		{
			ForceXAxis = 1.0f;
		}

		// ================ UP CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::W))
		{
			ForceZAxis = 1.0f;
		}

		// ================ DOWN CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::S))
		{
			ForceZAxis = -1.0f;
		}

		if (Input::InputSystem::GetInstance().GetKeyDown(Input::Key::Space))
		{
			ForceYAxis = 20.0f;

		}
		else
		{
			ForceYAxis = 0.0f;
		}

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftShift))
		{
			sprintMultiplier = m_SprintMultiplier;
		}
	

		
		Vector3 finalDirection = ((ForceZAxis * cameratransform.GetForwardVector()) +(ForceXAxis * cameratransform.GetRightVector()));
		finalDirection.y = 0.0f;
		

	
		if(ForceYAxis > 1.0f)
		{
			Vector3 velocity = m_rigidBodyRef->GetVelocity();
			velocity.y += ForceYAxis;
			m_rigidBodyRef->SetVelocity(velocity);
		}
		//LOG_WARN("{0} {1}", cameratransform.GetForwardVector().x, cameratransform.GetForwardVector().z);

		if (LengthSquared(finalDirection) < 0.1 )
		{
			
				
			m_rigidBodyRef->SetForce(Vector3(0.0f));
				
			if (leftHandAnimator)
			{
				
				leftHandAnimator->PlayAnimation("Idle");
			}
			if (rightHandAnimator)
			{
				
				rightHandAnimator->PlayAnimation("Idle");
			}
			return;
		}

		Vector3 targetDirection = Normalize(finalDirection);
		
		finalDirection = targetDirection * 160.0f * m_moveSpeed * sprintMultiplier;
		m_rigidBodyRef->SetForce(finalDirection);



		if (leftHandAnimator)
		{
			
			leftHandAnimator->PlayAnimation("Walk");
		}
		if (rightHandAnimator)
		{
			
			rightHandAnimator->PlayAnimation("Walk");
		}
		
	}


	void PlayerCharacter::AnimateHands()
	{


	}
	void PlayerCharacter::ProcessMouseInput()
	{
	}
}