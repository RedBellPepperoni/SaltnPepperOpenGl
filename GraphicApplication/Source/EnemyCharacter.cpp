#include "EnemyCharacter.h"
namespace SaltnPepperEngine
{
	void EnemyCharacter::UpdateState(const float deltaTime)
	{
		float duration = 0.0f;

		


		switch (currentState)
		{
		case EnemyState::IDLE:


			m_animator->PlayAnimationbyName("Idle");
			m_animator->SetTransitiontime(0.1f);

			break;

		case EnemyState::WALKING:

			m_animator->PlayAnimationbyName("Walk");
			m_animator->SetTransitiontime(0.1f);
			break;

		case EnemyState::ATTACKING:

			m_animator->PlayAnimationbyName("Attack");
			m_animator->SetTransitiontime(0.1f);
			m_counter += deltaTime;
			duration = 1.0f;
			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::IDLE;
			}

			break;


		case EnemyState::TAKINGHIT:

			
			m_animator->PlayAnimationbyName("HitOne",false);

			m_counter += deltaTime;
			duration = 0.6f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::IDLE;
			}

			break;

		case EnemyState::DYING:

		
			m_counter += deltaTime;
			duration = 3.00f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::DEAD;
			}

			break;
		}
	}

	void EnemyCharacter::DetectPlayer(const Vector3& position,const Vector3& playerpos)
	{
		const float sqDist = DistanceSquared(position, playerpos);
		
		if (sqDist > m_detectionRadius * m_detectionRadius) { return; }
		
		currentBehaviour = EnemyBehaviour::HUNT;
		

		if (sqDist < m_attackRadius * m_attackRadius)
		{
			currentBehaviour = EnemyBehaviour::ATTACK;
		}

	}

	void EnemyCharacter::DecideMovement(const Vector3& currentPosition,const Vector3& playerPosition, const Vector3& currentForward)
	{
		Vector3 finalDirection = Vector3(0.0f);


		switch (currentBehaviour)
		{
		case EnemyBehaviour::WANDER:

			break;

		case EnemyBehaviour::HUNT:

			finalDirection = Normalize(playerPosition - currentPosition);
			Move(finalDirection);

			break;
		}
	}

	void EnemyCharacter::Move(const Vector3& targetDirection)
	{	
		const Vector3 finalVelocity = targetDirection * m_speed * m_forceMultiplier;
		m_rigidBody->SetLinearVelocity(finalVelocity);
	}

	void EnemyCharacter::Attack(const Vector3& origin, const Vector3& target)
	{
		LOG_INFO("ATATCKED");

		float rayFraction = 0.0f;
		RigidBody* hitbody = PhysicsUtils::RayCast(origin, target, rayFraction, CollisionMask::STATIC);

		if (hitbody)
		{
			std::string name = hitbody->GetEntityId().GetComponent<NameComponent>().name;

			LOG_CRITICAL("HIT : {0}", name);

			//Entity hitEntity = hitbody->GetEntityId();

			//EnemyComponent* enemyComp = hitEntity.TryGetComponent<EnemyComponent>();

			//// Enemy Hit
			//if (enemyComp)
			//{
			//	EnemyCharacter* enemy = enemyComp->GetEnemy();
			//	enemy->TakeDamage(20);
			//}

		}

	}

	void EnemyCharacter::Die()
	{
		m_animator->PlayAnimationbyName("DeathOne",false);
		m_counter = 0.0f;
		currentState = EnemyState::DYING;
	}

	EnemyCharacter::EnemyCharacter()
	{	
		m_maxHealth = 100;
		m_health = m_maxHealth;



	}

	void EnemyCharacter::SetType(ZombieType type)
	{
		currentType = type;
	}

	void SaltnPepperEngine::EnemyCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
	}
	SkinnedAnimator* EnemyCharacter::GetAnimator()
	{
		return m_animator;
	}

	void EnemyCharacter::SetWeaponTransform(Transform* weaponTransform)
	{
		m_weaponTransform = weaponTransform;
	}

	void EnemyCharacter::SetRigidBodyRef(RigidBody* bodyRef)
	{
		m_rigidBody = bodyRef;
	}

	void EnemyCharacter::OnUpdate(float deltaTime, const Vector3& playerPos, Transform& enemyTransform, Transform& lookTransform)
	{
		if (currentState == EnemyState::DEAD) { return; }

		UpdateState(deltaTime);
		m_animator->OnUpdate(deltaTime);

		const Vector3 currentPosition = enemyTransform.GetPosition();


		DetectPlayer(currentPosition,playerPos);

		if (currentBehaviour == EnemyBehaviour::ATTACK)
		{
			m_attackCounter += deltaTime;

			if (!m_canAttack) 
			{ 	
				if (m_attackCounter < m_attackCooldown) { return; }
			
				m_attackCounter = 0.0f;
				m_canAttack = true; 
			}
			else
			{
				currentState = EnemyState::ATTACKING;

				if (m_attackCounter > m_attackEventTimer)
				{
					m_attackCounter = 0.0f;

					
					Vector3 origin = currentPosition + (lookTransform.GetForwardVector());
					origin.y = 0.5f;
					Vector3 destination = currentPosition - (lookTransform.GetForwardVector() * 3.0f);
					destination.y = 0.5f;

					Attack(origin,destination);
					m_canAttack = false;

				}

				

			}
			

			

		}
		
	}

	


	void EnemyCharacter::TakeDamage(const int damage)
	{
		if (currentState == EnemyState::DYING || currentState == EnemyState::DEAD)
		{
			return;
		}

		const int damagedHealth = GetCurrentHealth() - damage;

		if (damagedHealth <= 0)
		{
			m_health = 0;
			Die();
			return;
		}

		m_health = damagedHealth;

		m_counter = 0.0f;

		m_animator->SetTransitiontime(0.05f);
		currentState = EnemyState::TAKINGHIT;


	}

	EnemyComponent::EnemyComponent()
	{
		enemyHandle = MakeShared<EnemyCharacter>();
	}

	EnemyComponent::~EnemyComponent()
	{
	}

	EnemyCharacter* EnemyComponent::GetEnemy()
	{
		return enemyHandle.get();
	}
}