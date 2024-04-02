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

		const Vector3 flattenedForward = Normalize(Vector3(forwardDirection.x, 0.0f, forwardDirection.z));
		const Vector3 flattenedRight = Normalize(Vector3(rightDirection.x, 0.0f, rightDirection.z));

		Vector3 finalDirection = Vector3{ 0.0f,0.0f,0.0f };
			
		// ============ FORWARD MOVEMENT =================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::W))
		{		
			//m_rigidBody->ApplyCentralForce(forwardDirection * finalSpeed);

			finalDirection += flattenedForward;
		}

		// ================ REVERSE MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::S))
		{	
			finalDirection += -1.0f * flattenedForward;
		}

		// ================ RIGHT CAMERA MOVEMENT =====================

		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::A))
		{
			finalDirection += -1.0f * flattenedRight;
		}

		// ================ LEFT CAMERA MOVEMENT =====================


		if (Input::InputSystem::GetInstance().GetKeyHeld(Input::Key::D))
		{
			finalDirection += flattenedRight;
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

	void PlayerCharacter::MouseInput(const float deltaTime, Vector2 mousePosition, Transform& lookTransform)
	{
		// Window is not in focus , don't do the mosue update
		/*if (!Application::GetCurrent().GetAppWindow().GetFocus())
		{
			return;
		}*/

		m_sensitivity = 0.0002f;



		m_rotationalVelocity = Vector2((mousePosition.x - m_previousCursorPosition.x), (mousePosition.y - m_previousCursorPosition.y)) * m_sensitivity * 10.0f;

		//Application::GetCurrent().GetAppWindow().SetMouseHidden(true);

		if (Length(m_rotationalVelocity) > 0.0001f)
		{
			//Get The Rotation
			Quaternion rotation = lookTransform.GetRotation();


			Quaternion Pitch = glm::angleAxis(-m_rotationalVelocity.y, Vector3(1.0f, 0.0f, 0.0f));


			Quaternion Yaw = glm::angleAxis(-m_rotationalVelocity.x, Vector3(0.0f, 1.0f, 0.0f));


			rotation = Yaw * rotation;
			rotation = rotation * Pitch;

		



			lookTransform.SetRotation(rotation);

			m_previousCursorPosition = mousePosition;





		}
		else
		{
			m_rotationalVelocity = Vector2(0.0f);
		}





		m_rotationalVelocity = m_rotationalVelocity * pow(m_rotationalDampening, deltaTime);


	}

	void PlayerCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
	}

	void PlayerCharacter::SetRigidBodyRef(RigidBody* bodyRef)
	{
		m_rigidBody = bodyRef;
	}

	void PlayerCharacter::OnUpdate(float deltaTime, Vector2 mousePosition, Transform& lookTransform)
	{
		if (Application::GetCurrent().GetEditorActive()) { return; }


		KeyBoardInput(deltaTime, lookTransform);
		MouseInput(deltaTime, mousePosition,lookTransform);

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
