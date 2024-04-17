#include "BuddyCharacter.h"
#include "GameManager.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	BuddyCharacter::BuddyCharacter()
	{
		m_entityManger = Application::GetCurrent().GetCurrentScene()->GetEntityManager();
	}
	BuddyCharacter::~BuddyCharacter()
	{

	}

	void BuddyCharacter::TakeDamage(const int damage)
	{
	
		//const int damagedHealth = GetCurrentHealth() - damage;

		/*if (damagedHealth <= 0)
		{
			m_health = 0;
			Die();
			return;
		}*/

		//m_health = damagedHealth;

		LOG_CRITICAL("Took Damage {0}", damage);

		m_counter = 0.0f;

		m_animator->SetTransitiontime(0.05f);
		currentState = BuddyState::TAKINGHIT;
	}

	void BuddyCharacter::UpdateTargetandPath(const Vector3& target, const std::vector<Vector3>& newpath)
	{
		if (newpath.empty()) { return; }

		if (newpath.size() == 1)
		{
			m_targetPosition = target;
			m_currentPath = newpath;

			m_currentWaypointIndex = 0;
			m_isfollowingPath = true;
			m_targetClose = true;
			return;
		}

		m_targetPosition = target;
		m_currentPath = newpath;

		m_currentWaypointIndex = 1;
		m_isfollowingPath = true;
		m_targetClose = false;

	}

	void BuddyCharacter::OnUpdate(const float& deltaTime, Transform& buddyTransform, Transform& lookTransform)
	{
		const Vector3 currentPosition = buddyTransform.GetPosition() - Vector3{0.0f,0.81,0.0f};
		const Vector3 currentForward = lookTransform.GetForwardVector();
		
		UpdateAnimState(deltaTime);

		m_animCounter += deltaTime;

		m_attackCounter -= deltaTime;

		if (m_attackCounter <= 0.0f)
		{
			m_attackCounter = 0.0f;
		}


		if (m_animCounter >= animationFrameRate)
		{
			m_animCounter = 0.0f;
			m_animator->OnUpdate(animationFrameRate);

		}

		if (currentState == BuddyState::TAKINGHIT) { return; }

		if (!m_isfollowingPath)
		{
			
			DetectEnemyTarget(deltaTime, buddyTransform,lookTransform );

			

			if (currentBehaviour == BuddyBehaviour::ATTACK)
			{
				if (m_canAttack == false)
				{
					if (m_attackCounter > 0.0f) { return; }

					m_attackCounter = m_attackEventTimer;
					m_canAttack = true;
				}
				else
				{
					currentState = BuddyState::ATTACKING;

					if (m_attackCounter > 0.0f)
					{
						return;
					}
					
					m_attackCounter = m_attackCooldown;

					Vector3 origin = currentPosition - (currentForward) * 0.01f;
					origin.y = origin.y + 0.5f;
					Vector3 destination = currentPosition - (currentForward * 2.0f);
					destination.y = destination.y + 0.5f;

					Attack(origin, destination);
					m_canAttack = false;
					

				}
			}
			
	
		}

		else
		{
			if (!m_targetClose)
			{
				const Vector3 currentWaypoint = GetCurrentWayPoint();

				float waypointDistance = DistanceSquared(currentWaypoint, currentPosition);

				if (waypointDistance < 0.1f)
				{
					SetNextWaypoint();
				}

				else
				{
					const Vector3 direction = Normalize(currentWaypoint - currentPosition);
					Move(direction);
					RotateModel(deltaTime, direction, lookTransform);
				}
			}

			else
			{

				

				const float distance = DistanceSquared(m_targetPosition, currentPosition);


				if (distance > 0.1f)
				{
					const Vector3 direction = Normalize(m_targetPosition - currentPosition);
					Move(direction);
					RotateModel(deltaTime, direction, lookTransform);

				}

				else
				{
					m_isfollowingPath = false;
					m_targetClose = false;
					currentState = BuddyState::IDLE;
					m_gameManagerRef->HideMarker();
				}
				
				
	

			}
		}


	}

	void BuddyCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
	}

	SkinnedAnimator* BuddyCharacter::GetAnimator()
	{
		return m_animator;
	}

	void BuddyCharacter::SetRigidBodyRef(RigidBody* bodyRef)
	{
		m_rigidBody = bodyRef;
	}

	void BuddyCharacter::SetGameManagerRef(GameManager* finder)
	{
		m_gameManagerRef = finder;
	}

	void BuddyCharacter::UpdateAnimState(const float deltaTime)
	{
		float duration = 0.0f;

		DebugRenderer::DrawLine(m_attackOrigin,m_targetOrigin,Vector4(1.0f,0.0f,0.0f,1.0f));


		switch (currentState)
		{
		case BuddyState::IDLE:

			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("Idle");



			break;

		case BuddyState::WALKING:
			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("Walk");

			//PlayIdleSound();

			break;

		case BuddyState::ATTACKING:
			m_animator->SetTransitiontime(0.2f);
			m_animator->PlayAnimationbyName("Attack");

			m_counter += deltaTime;
			duration = 0.9f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = BuddyState::IDLE;
			}

			break;


		case BuddyState::TAKINGHIT:

			m_animator->SetTransitiontime(0.05f);
			m_animator->PlayAnimationbyName("Hit", false);

			m_counter += deltaTime;
			duration = 0.6f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = BuddyState::IDLE;
			}

			break;

		}
	}
		

	void BuddyCharacter::Move(const Vector3& targetDirection)
	{
		Vector3 finalVelocity = targetDirection * m_speed * m_forceMultiplier;
		float fallVelo = m_rigidBody->GetLinearVelocity().y;

		finalVelocity.y = fallVelo;
		m_rigidBody->SetLinearVelocity(finalVelocity);

		currentState = BuddyState::WALKING;
	}

	void BuddyCharacter::RotateModel(float deltatime, const Vector3& target, Transform& looktransform)
	{
		Vector3 forward = looktransform.GetForwardVector();
		float rotationStep = m_turnRate * deltatime;

		float angle = atan2(Length(Cross(target, forward)), Dot(target, forward));
		if (angle < rotationStep) { return; }

		Vector3 rotationAxis = Cross(forward, target);

		if (rotationAxis.y > 0.01f)
		{
			rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			rotationAxis = Vector3(0.0f, -1.0f, 0.0f);
		}

		Quaternion rotation = Rotate(looktransform.GetRotation(), -rotationStep, rotationAxis);
		looktransform.SetRotation(rotation);
	}

	void BuddyCharacter::Attack(const Vector3& origin, const Vector3& target)
	{
		LOG_WARN("BUDDY ATTACKS");

		m_attackOrigin = origin;
		m_targetOrigin = target;

		//LOG_ERROR("Origin : {0} {1} {2} : Target : {3} {4} {5}", origin.x, origin.y, origin.z, target.x, target.y, target.z);

		Vector3 hitPosition;
		RigidBody* hitbody = PhysicsUtils::RayCast(origin, target, hitPosition, CollisionMask::AllFilter);


		
		//PlayAttackSound();

		if (hitbody)
		{
			

			Entity hitEntity = hitbody->GetEntityId();

			EnemyComponent* enemyComp = hitEntity.TryGetComponent<EnemyComponent>();

			std::string name = hitEntity.TryGetComponent<NameComponent>()->name;

			// Enemy Hit
			if (enemyComp)
			{
				EnemyCharacter* enemy = enemyComp->GetEnemy();
				enemy->TakeDamage(20);
			}

		}


	}

	void BuddyCharacter::DetectEnemyTarget(const float deltaTime, Transform& currTransform, Transform& lookTransform)
	{
		// Doesnt have enemy target : Do nothing
		if (m_hasEnemy == false) 
		{ 
			currentBehaviour = BuddyBehaviour::DECIDING;
			currentState = BuddyState::IDLE;
			return;
		}

		if (m_isfollowingPath) { return; }

		const Vector3 currentPos = currTransform.GetPosition() - Vector3{ 0.0f,0.8f,0.0f };

		const float distance = DistanceSquared(m_enemyPosition, currentPos);

		// We have Detected Enemy
		if (distance < Square(m_detectionRadius))
		{
			//m_targetPosition = m_enemyPosition;

			Vector3 direction = (m_enemyPosition - currentPos);
			direction.y = 0.0f;

			RotateModel(deltaTime, Normalize(direction), lookTransform);

			if (distance < Square(m_attackRadius))
			{
				currentBehaviour = BuddyBehaviour::ATTACK;
			}
			else
			{
				//m_canAttack = false;
			}

		}

	}

	

	void BuddyCharacter::Die()
	{

	}

	void BuddyCharacter::SetMarkedEnemy(const Vector3& position,const bool hasTarget)
	{
		m_enemyPosition = position;
		m_hasEnemy = hasTarget;
	}

	void BuddyCharacter::SetNextWaypoint()
	{
		m_currentWaypointIndex++;

		if (m_currentWaypointIndex == m_currentPath.size())
		{
			m_currentPath.clear();
			m_targetClose = true;
			
		}

		if (m_currentWaypointIndex < 0 || m_currentWaypointIndex >= m_currentPath.size()) 
		{ 
			m_currentWaypointIndex = 0; 

		}
	}

	Vector3 BuddyCharacter::GetCurrentWayPoint()
	{
	
		if (m_currentPath.empty() || m_currentWaypointIndex < 0 || m_currentWaypointIndex >= m_currentPath.size())
		{
			return Vector3{ 0.0f };
		}

		return m_currentPath[m_currentWaypointIndex];
	}



	BuddyComponent::BuddyComponent()
	{
		m_buddyHandle = MakeShared<BuddyCharacter>();
	}

	BuddyComponent::~BuddyComponent()
	{

	}

	BuddyCharacter* BuddyComponent::GetBuddy()
	{
		return m_buddyHandle.get();
	}
}
