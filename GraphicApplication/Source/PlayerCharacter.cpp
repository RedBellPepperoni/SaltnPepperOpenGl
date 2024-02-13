#include "PlayerCharacter.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/System/Input/InputSystem.h"
#include "Engine/Core/System/Application/Application.h"

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

	void PlayerCharacter::Update(float deltaTime)
	{
		//ProcessKeyboardInput();
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
				//m_rigidBodyRef->SetVelocity(m_rigidBodyRef->GetVelocity() * 0.98f);
				return;
			

		}

		Vector3 targetDirection = Normalize(finalDirection);
		
		finalDirection = targetDirection * 160.0f * m_moveSpeed * sprintMultiplier;
		m_rigidBodyRef->SetForce(finalDirection);

	/*	float rotationStep = 5.1f * deltaTime;
		
		Vector3 forwardDirection = Normalize(m_lookTransfrom->GetForwardVector());
		float angle = glm::angle(targetDirection, forwardDirection);

		if (angle < rotationStep) { return; }

		Vector3 rotationAxis = Cross(forwardDirection, targetDirection);

		if (rotationAxis.y > 0.01f)
		{
			rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			rotationAxis = Vector3(0.0f, -1.0f, 0.0f);
		}

		Quaternion rotation = GetQuaternion(Rotate(m_lookTransfrom->GetLocalMatrix(), rotationStep, rotationAxis));
		
		m_lookTransfrom->SetRotation(rotation);*/
		
	}


	void PlayerCharacter::AnimateHands()
	{


	}
	void PlayerCharacter::ProcessMouseInput()
	{
	}
}