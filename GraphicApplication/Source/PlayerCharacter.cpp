#include "PlayerCharacter.h"

namespace SaltnPepperEngine
{
	void PlayerCharacter::KeyBoardInput(const float deltaTime, Transform& lookTransform)
	{
		// Defalut speed
		//m_speed = 80.0f * deltaTime;

		if (m_rigidBody == nullptr) { return; }

		float finalSpeed = m_speed;

		// Way more if needed
		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftShift))
		{
			finalSpeed = m_speed * 1.6f;
		}

		Vector3 forwardDirection = lookTransform.GetForwardVector();


		// ============ FORWARD CAMERA MOVEMENT =================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::W))
		{		
			m_rigidBody->ApplyCentralForce(forwardDirection * finalSpeed);
		}

		// ================ REVERSE CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::S))
		{
			

		}

		// ================ RIGHT CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::A))
		{
			
		}

		// ================ LEFT CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::D))
		{
			
		}

		// ================ UP CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::Space))
		{
			
		}

		// ================ DOWN CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::LeftControl))
		{
			
		}


		// If there is velocity, move the Object

		/*if (m_velocity.length() > 0.00001f)
		{
			Vector3 position = transform.GetPosition();
			position += m_velocity * deltaTime;
			transform.SetPosition(position);
			m_velocity = m_velocity * pow(m_linearDampening, deltaTime);
		}

		else
		{
			m_velocity = Vector3(0.0f);

		}*/
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
