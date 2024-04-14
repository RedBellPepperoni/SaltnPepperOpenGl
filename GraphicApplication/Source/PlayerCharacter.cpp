#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

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

		if (InputSystem::GetInstance().GetKeyDown(Key::F))
		{
			const Vector3 origin = lookTransform.GetWorldPosition();
			const Vector3 forward = lookTransform.GetForwardVector();

			// Raycast check if the hit object was an enemy
			RayCastbyTag(origin, forward, EnvironmentTag::Tag::NAVMESH);
		}


		if (InputSystem::GetInstance().GetKeyDown(Key::R))
		{
			isReloading = true;
		}


		if (isReloading)
		{
			m_reloadCounter += deltaTime;

			m_animator->PlayAnimationbyName("Reload");

			if (m_reloadCounter > m_ReloadTimer)
			{
				isReloading = false;
				m_reloadCounter = 0.0f;
				
			}

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


		m_shootcounter += deltaTime;

		static bool LeftButtonHeld = false;

		if(InputSystem::GetInstance().GetMouseBtnClicked(Input::MouseButton::Left) && m_shootcounter > m_shootCooldown && !isReloading)
		{
			LeftButtonHeld = true;

			m_animator->PlayAnimationbyName("Shoot",false);
			m_shootcounter = 0.0f;

			//LOG_CRITICAL("SHOOT");

			const Vector3 origin = lookTransform.GetWorldPosition();
			const Vector3 forward = lookTransform.GetForwardVector();

			// Raycast check if the hit object was an enemy
			RayCastbyTag(origin, forward, EnvironmentTag::Tag::ENEMY);

		}


		if (m_shootcounter > m_shootCooldown + m_shootCooldown && !isReloading)
		{
			m_animator->PlayAnimationbyName("Idle", true);
		}

		//if (!LeftButtonHeld) { m_animator->PlayAnimationbyName("Idle", true); }

	}

	bool PlayerCharacter::RayCastbyTag(const Vector3& origin, const Vector3& forward, EnvironmentTag::Tag tag)
	{
		const Vector3 offsetOrigin = origin + (forward * (0.4f));
		const Vector3 targetOffset = origin + (forward * (50.0f));

		Vector3 hitPosition = Vector3{0.0f,0.0f,0.0f};

		RigidBody* hitbody = PhysicsUtils::RayCast(offsetOrigin, targetOffset, hitPosition, CollisionMask::AllFilter);

		// Nothing was hit just return back
		if (hitbody == nullptr)
		{
			return false;
		}


		// ================== RIGIDBODY HIT ============================== 

		// get the parent entity from teh body that was hit
		Entity hitEntity = hitbody->GetEntityId();

		const EnvironmentTag::Tag hitBodyTag = hitEntity.GetComponent<EnvironmentTag>().currentTag;

		if (hitBodyTag != tag)
		{
			return false;
		}


		// ============== CORRECT TAGGED ENTITY WAS HIT ===================


		switch (tag)
		{
			case SaltnPepperEngine::EnvironmentTag::Tag::NONE:
			{

			}
			break;

			case SaltnPepperEngine::EnvironmentTag::Tag::ENEMY:
			{
				EnemyComponent* enemyComp = hitEntity.TryGetComponent<EnemyComponent>();
				if (enemyComp)
				{
					EnemyCharacter* enemy = enemyComp->GetEnemy();
					enemy->TakeDamage(20);
					return true;
				}

			}
			break;

			case SaltnPepperEngine::EnvironmentTag::Tag::BUDDY:
			{

			}

			break;

			case SaltnPepperEngine::EnvironmentTag::Tag::NAVMESH:
			{
				LOG_CRITICAL("Position Hit : {0} , {1} , {2}", hitPosition.x, hitPosition.y, hitPosition.z);
			}
			
			break;


			case SaltnPepperEngine::EnvironmentTag::Tag::LIGHT:
			{

			}
			break;

			case SaltnPepperEngine::EnvironmentTag::Tag::TV:
			{

			}
			break;
		
		}


		


		

	}

	

	void PlayerCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
		m_animator->SetTransitiontime(0.01f);
	}

	SkinnedAnimator* PlayerCharacter::GetAnimator()
	{
		return m_animator;
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

		m_animator->OnUpdate(deltaTime);

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
