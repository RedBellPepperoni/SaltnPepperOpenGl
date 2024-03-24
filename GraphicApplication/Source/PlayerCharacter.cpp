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

	void PlayerCharacter::Init(RigidBody3D* RigidBodyRef,Transform* cameraTransformRef)
	{
		m_rigidBodyRef = RigidBodyRef;
		m_cameraRef = cameraTransformRef;
		//m_groundDetector = groundDetect;


		m_rigidBodyRef->m_OnCollisionCallback = std::bind(&PlayerCharacter::DetectGround, this, std::placeholders::_1, std::placeholders::_2);
		/*m_leftHand = leftHand;
		m_rightHand = rightHand;*/


	}

	void PlayerCharacter::SetAudioSource(AudioSource* Source)
	{
		m_source = Source;
	}

	void PlayerCharacter::SetAnimators(AnimationComponent* left, AnimationComponent* right)
	{
		leftHandAnimator = left;
		rightHandAnimator = right;
	}

	void PlayerCharacter::Update(float deltaTime)
	{
		//float velocityDown = m_rigidBodyRef->GetVelocity().y;
		
		/*if (!m_rigidBodyRef->isColliding)
		{
			canJump = false;
		}*/
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
		//m_groundDetector->SetPosition(m_rigidBodyRef->GetPosition());

		if (!canLegCharge)
		{
			timeCounter += deltaTime;

			if (timeCounter > doomChargeCooldown)
			{
				timeCounter = 0;
				canLegCharge = true;
			}
		}


		
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
		

	
		if(ForceYAxis > 1.0f && canJump)
		{

			Vector3 velocity = m_rigidBodyRef->GetVelocity();
			velocity.y += ForceYAxis;
			m_rigidBodyRef->SetVelocity(velocity);

			canJump = false;


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


	bool PlayerCharacter::DetectGround(RigidBody3D* thisBody, RigidBody3D* otherBody)
	{
		
		if (otherBody->m_tag & CollisionTag::PLATFORM)
		{
			canJump = true;



		}
		else if (otherBody->m_tag & CollisionTag::PLATFORM)
		{
			canJump = true;


		}

		if (otherBody->m_tag & (CollisionTag::BOOMERANG))
		{
			
			canJump = true;
			//LOG_WARN("Ground Detected");


		}
	


		return true;
	}

	void PlayerCharacter::AnimateHands()
	{


	}

	void PlayerCharacter::ProcessMouseInput(Transform& cameratransform,float Deltatime)
	{
		static bool mouseHeld = false;
		

		if (Input::InputSystem::GetInstance().GetMouseBtnClicked(MouseButton::Left))
		{
			mouseHeld = true;
		}
	
		
		if (Input::InputSystem::GetInstance().GetMouseBtnHeld(MouseButton::Left))
		{
			ChargeAmount += Deltatime;
			if (ChargeAmount > maxCharge)
			{
				ChargeAmount = maxCharge;
			}
		}
		else
		{
			if (mouseHeld)
			{
				mouseHeld = false;

				if (ChargeAmount >= 0.01f && canJump)
				{
					DoomLegLaunch(cameratransform.GetForwardVector());
				}
			}

		}
		
	}

	void PlayerCharacter::DoomLegLaunch(Vector3 LookDirection)
	{


		Vector3 velocity = m_rigidBodyRef->GetVelocity();
		Vector3 direction = Normalize(Normalize(LookDirection) + Vector3(0.0f, 1.0f, 0.0f));

		/*if (LengthSquared(velocity) < 0.2f)
		{
			
			direction = Normalize(Normalize(LookDirection) + Vector3(0.0f, 1.0f, 0.0f));

		}
		else
		{
			direction = Normalize(velocity);
			if (direction.y < 0.1f)
			{
				direction.y = 1.0f;
			}
			direction = Normalize(direction);
		}*/


		velocity += direction * ChargeAmount * 30.0f;

		//velocity.y +=  ChargeAmount * 30.0f;
		m_rigidBodyRef->SetVelocity(velocity);

		if (m_source)
		{
			m_source->PlayClip();
		}

		canJump = false;
		canLegCharge = false;

	}
}