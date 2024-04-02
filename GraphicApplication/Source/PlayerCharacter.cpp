#include "PlayerCharacter.h"

namespace SaltnPepperEngine
{
	void PlayerCharacter::KeyBoardInput(const float deltaTime, Transform& lookTransform)
	{
		// Defalut speed
		//m_speed = 80.0f * deltaTime;

		if (m_rigidBody == nullptr) { return; }

		float finalSpeed = m_speed * m_forceMultiplier;

		// Way more if needed
		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftShift))
		{
			finalSpeed = m_speed * 1.6f * m_forceMultiplier;
		}

		const Vector3 forwardDirection = lookTransform.GetForwardVector();
		const Vector3 rightDirection = lookTransform.GetRightVector();

		Vector3 finalDirection = Vector3{ 0.0f,0.0f,0.0f };
			
		// ============ FORWARD MOVEMENT =================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::W))
		{		
			//m_rigidBody->ApplyCentralForce(forwardDirection * finalSpeed);

			finalDirection += forwardDirection ;
		}

		// ================ REVERSE MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::S))
		{	
			finalDirection += -1.0f * forwardDirection ;
		}

		// ================ RIGHT CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::A))
		{
			finalDirection += -1.0f * rightDirection;
		}

		// ================ LEFT CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::D))
		{
			finalDirection +=  rightDirection ;
		}

		// ================ UP CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Space))
		{
			
		}

		// ================ DOWN CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftControl))
		{
			
		}

		Vector3 verticalVelocity = m_rigidBody->GetLinearVelocity();

		if (LengthSquared(finalDirection) > 0.1f)
		{
			finalDirection = Normalize(finalDirection);
			
			Vector3 horizontalVelocity = (finalDirection * finalSpeed);
			Vector3 finalVelo = Vector3{ horizontalVelocity.x,verticalVelocity.y,horizontalVelocity.z };
			m_rigidBody->SetLinearVelocity(finalVelo);
		}
		else
		{
			m_rigidBody->SetLinearVelocity(Vector3{0.0f,verticalVelocity.y,0.0f});
		}

		
	}

	void PlayerCharacter::MouseInput(const float deltaTime, Transform& lookTransform)
	{
	}

	void PlayerCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
	}

	void PlayerCharacter::SetRigidBodyRef(RigidBody* bodyRef)
	{
		m_rigidBody = bodyRef;
	}

	void PlayerCharacter::OnUpdate(float deltaTime, Transform& lookTransform)
	{
		if (Application::GetCurrent().GetEditorActive()) { return; }


		KeyBoardInput(deltaTime, lookTransform);
		MouseInput(deltaTime, lookTransform);

	}

	void PlayerCharacter::TakeDamage(const int damage)
	{


	}


	PlayerComponent::PlayerComponent()
	{
		m_playerhandle = MakeShared<PlayerCharacter>();
	}

	PlayerComponent::~PlayerComponent()
	{

	}

	PlayerCharacter* PlayerComponent::GetPlayer()
	{
		return m_playerhandle.get();
	}
}
