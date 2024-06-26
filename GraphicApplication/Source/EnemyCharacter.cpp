#include "EnemyCharacter.h"
#include "GameManager.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	void EnemyCharacter::UpdateState(const float deltaTime)
	{
		float duration = 0.0f;

		if (m_markedForDeath)
		{
			m_deathcounter += deltaTime;
			duration = 3.3f;

			if (m_deathcounter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::DEAD;
				m_rigidBody->MakeTrigger();
				//m_rigidBody->SetActivationState(ActivationState::DISABLESIMULATION);
			}

			return;
		}


		DebugRenderer::DrawLine(m_attackorigin, m_attacktarget, Vector4(1.0f, 0.0f, 0.0f, 1.0f));


		switch (currentState)
		{
		case EnemyState::IDLE:

			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("Idle");
			
			

			break;

		case EnemyState::WALKING:
			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("Walk");

			//PlayIdleSound();
			
			break;

		case EnemyState::ATTACKING:
			m_animator->SetTransitiontime(0.2f);
			m_animator->PlayAnimationbyName("Attack");
			
			m_counter += deltaTime;
			duration = 1.0f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::IDLE;
			}

			break;


		case EnemyState::TAKINGHIT:

			m_animator->SetTransitiontime(0.05f);
			m_animator->PlayAnimationbyName("HitOne",false);

			m_counter += deltaTime;
			duration = 0.6f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = EnemyState::IDLE;
			}

			break;

	
			
		}
	}

	void EnemyCharacter::DetectPlayerandBuddy(const float deltaTime, const Vector3& position,const Vector3& playerpos, const Vector3& buddypos)
	{
		

		const float sqDistplayer = DistanceSquared(position, playerpos);
		const float sqDistbuddy = DistanceSquared(position, buddypos);

		m_playerCloser = sqDistplayer < sqDistbuddy;
		
		
		

		if ((m_playerCloser ? sqDistplayer : sqDistbuddy) > m_detectionRadius * m_detectionRadius)
		{ 
			currentBehaviour = EnemyBehaviour::DECIDING;
			m_shouldScream = true;
			return; 
		}

		if (m_canScream && m_shouldScream)
		{
			screamCounter = 0.0f;
			PlayScreamSound();
			m_canScream = false;
			m_shouldScream = false;
		}

		else
		{
			screamCounter += deltaTime;

			//LOG_INFO("Counter : {0}",attackSreamcounter);

			if (screamCounter > 2.50)
			{
				m_canScream = true;
			}

			//PlayIdleSound();
		}

		
		currentBehaviour = EnemyBehaviour::HUNT;
		

		if ((m_playerCloser ? sqDistplayer : sqDistbuddy) < m_attackRadius * m_attackRadius)
		{
			currentBehaviour = EnemyBehaviour::ATTACK;
		}

	}

	void EnemyCharacter::DecideMovement(const float deltatime,const Vector3& currentPosition,const Vector3& playerPosition, const Vector3& buddyPosition, Transform& lookTrasform)
	{
		Vector3 finalDirection = Vector3(0.0f);

		

		switch (currentBehaviour)
		{
		case EnemyBehaviour::DECIDING:
			
			currentState = EnemyState::IDLE;
			break;

		case EnemyBehaviour::WANDER:

			break;

		case EnemyBehaviour::HUNT:

			
			finalDirection = Normalize((m_playerCloser? playerPosition : buddyPosition) - currentPosition);
			Move(finalDirection);

			RotateModel(deltatime,finalDirection, lookTrasform);

			break;
		}
	}

	void EnemyCharacter::Move(const Vector3& targetDirection)
	{	
		Vector3 finalVelocity = targetDirection * m_speed * m_forceMultiplier;
		float fallVelo = m_rigidBody->GetLinearVelocity().y;

		finalVelocity.y = fallVelo;
		m_rigidBody->SetLinearVelocity(finalVelocity);

		currentState = EnemyState::WALKING;
	}

	void EnemyCharacter::RotateModel(float deltatime, const Vector3& target, Transform& looktransform)
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


		//Quaternion rot = LookAtRotation(-target, Vector3(0.0f, 1.0f, 0.0f));
		//Quaternion currentRot = looktransform.GetRotation();

		//Quaternion finalRot = Slerp

		//looktransform.SetRotation(rot);
	}



	void EnemyCharacter::Attack(const Vector3& origin, const Vector3& target)
	{

		m_attackorigin = origin;
		m_attacktarget = target;

		Vector3 hitPosition;
		RigidBody* hitbody = PhysicsUtils::RayCast(origin, target, hitPosition, CollisionMask::AllFilter);

		PlayAttackSound();

		if (hitbody)
		{

			Entity hitEntity = hitbody->GetEntityId();

			// PLayer Hit
			PlayerComponent* playerComp = hitEntity.TryGetComponent<PlayerComponent>();

			if (playerComp)
			{
				PlayerCharacter* player = playerComp->GetPlayer();
				player->TakeDamage(10, DamageSource::ZOMBIE);
				return;
			}

			// Buddy Hit
			BuddyComponent* buddyComp = hitEntity.TryGetComponent<BuddyComponent>();

			if (buddyComp)
			{
				BuddyCharacter* buddy = buddyComp->GetBuddy();
				buddy->TakeDamage(5, DamageSource::ZOMBIE);
			}

		}

	}

	void EnemyCharacter::Die()
	{
		PlayDeathSound();
		m_animator->PlayAnimationbyName("DeathOne",false);
		m_deathcounter = 0.0f;
		m_markedForDeath = true;
		m_idlesource->StopPlayback();

		m_gameManager->SetEnemyDeath(m_rigidBody);
		MarkforBuddy(false);
	}

	void EnemyCharacter::PlayIdleSound()
	{
		LOG_WARN("IDLESOUND PLaying");
		//m_source->PlayClipLoop(m_idleclip);

	}

	void EnemyCharacter::PlayScreamSound()
	{

		m_source->PlayOneShot(m_alertclip);
		currentSound = SoundType::SCREAMSOUND;
	}

	void EnemyCharacter::PlayAttackSound()
	{
		m_source->PlayOneShot(m_attackclip);
		currentSound = SoundType::ATTACKSOUND;
	}

	void EnemyCharacter::PlayDeathSound()
	{
		m_source->PlayOneShot(m_deathclip);
		currentSound = SoundType::DEATHSOUND;
	}

	EnemyCharacter::EnemyCharacter()
	{	
		m_maxHealth = 100;
		m_health = m_maxHealth;



	}

	void EnemyCharacter::BeginReset(Transform transfomr)
	{
		//m_rigidBody->
		m_rigidBody->MakeDynamic();
		m_rigidBody->ForceTransform(transfomr);
		m_rigidBody->ApplyCentralForce(Vector3(1.0f));
		currentBehaviour = EnemyBehaviour::DECIDING;
		currentState = EnemyState::IDLE;
		m_markedForDeath = false;
		m_health = m_maxHealth;

	}

	void EnemyCharacter::SetMarkRef(Entity mark)
	{
		markEntity = mark;
	}

	void EnemyCharacter::SetType(ZombieType type)
	{
		currentType = type;
	}

	void EnemyCharacter::SetGameManagerRef(GameManager* finder)
	{
		m_gameManager = finder;
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

	void EnemyCharacter::SetAudioSource(Audio::AudioSource* source, Audio::AudioSource* idleSource)
	{
		m_source = source;
		m_idlesource = idleSource;


		m_source->SetAudioClip(m_alertclip);
		m_source->SetLoop(false);

		m_source->SetAudioClip(m_deathclip);
		m_source->SetLoop(false);

		m_source->SetAudioClip(m_attackclip);
		m_source->SetLoop(false);
	}

	void EnemyCharacter::SetAudioClips(AudioClip* idle, AudioClip* alert,  AudioClip* attack, AudioClip* death)
	{
		m_idleclip = idle;
		m_alertclip = alert;
		m_attackclip = attack;
		m_deathclip = death;

	}

	void EnemyCharacter::OnUpdate(float deltaTime, const Vector3& playerPos, const Vector3& buddyPos, Transform& enemyTransform, Transform& lookTransform)
	{
		if (currentState == EnemyState::DEAD) { return; }

		UpdateState(deltaTime);

		m_animCounter += deltaTime;

		if (m_animCounter >= animationFrameRate)
		{
			m_animCounter = 0.0f;
			m_animator->OnUpdate(animationFrameRate);
		}
		
	

		if (currentState == EnemyState::TAKINGHIT || m_markedForDeath) { return; }

		const Vector3 currentPosition = enemyTransform.GetPosition();
		const Vector3 currentForward = lookTransform.GetForwardVector();

		DetectPlayerandBuddy(deltaTime,currentPosition,playerPos,buddyPos);
		DecideMovement(deltaTime,currentPosition,playerPos,buddyPos, lookTransform);

		if (currentBehaviour == EnemyBehaviour::ATTACK)
		{
			m_attackCounter -= deltaTime;

			if (m_attackCounter <= 0.0f)
			{
				m_attackCounter = 0.0f;
			}

			if (!m_canAttack) 
			{ 	
				if (m_attackCounter > 0.0f) { return; }
			
				m_attackCounter = m_attackEventTimer;
				m_canAttack = true; 
			}
			else
			{
				currentState = EnemyState::ATTACKING;

				if (m_attackCounter > 0.0f)
				{
					return;
				}
				
				m_attackCounter = m_attackCooldown;

				Vector3 origin = currentPosition - (currentForward) * 0.01f;
				origin.y += 0.2f;
				Vector3 destination = currentPosition - (currentForward * 2.0f);
				destination.y += 0.2f;

				Attack(origin, destination);
				m_canAttack = false;
					

				

			}
			

			

		}
		
	}

	


	void EnemyCharacter::TakeDamage(const int damage, const DamageSource& source)
	{
		if (m_markedForDeath || currentState == EnemyState::DEAD)
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

	void EnemyCharacter::MarkforBuddy(bool mark)
	{
		m_markedbyBuddy = mark;
		// Add mark enable disable here

		markEntity.SetActive(mark);
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