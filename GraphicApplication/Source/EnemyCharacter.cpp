#include "EnemyCharacter.h"
namespace SaltnPepperEngine
{
	void EnemyCharacter::UpdateState(const float deltaTime)
	{
		

		switch (currentState)
		{
		case SaltnPepperEngine::EnemyCharacter::IDLE:

			m_animator->PlayAnimationbyName("Idle");

			break;

		case SaltnPepperEngine::EnemyCharacter::WALKING:

			m_animator->PlayAnimationbyName("Walk");

			break;
		case SaltnPepperEngine::EnemyCharacter::SHOOTING:

			m_animator->PlayAnimationbyName("Shoot");

			break;
		case SaltnPepperEngine::EnemyCharacter::RELOADING:

			break;

		case SaltnPepperEngine::EnemyCharacter::TAKINGHIT:

			m_animator->PlayAnimationbyName("TakeHit");

			m_counter += deltaTime;

			break;



			break;
		default:
			break;
		}
	}

	void EnemyCharacter::Move()
	{
	}

	void EnemyCharacter::Die()
	{
		m_animator->PlayAnimationbyName("Death");

		currentState = EnemyState::DEAD;
	}

	void SaltnPepperEngine::EnemyCharacter::SetAnimatorRef(SkinnedAnimator* animRef)
	{
		m_animator = animRef;
	}
	SkinnedAnimator* EnemyCharacter::GetAnimator()
	{
		return m_animator;
	}

	void EnemyCharacter::SetRigidBodyRef(RigidBody* bodyRef)
	{
		m_rigidBody = bodyRef;
	}

	void EnemyCharacter::OnUpdate(float deltaTime, const Vector3& playerHeadPos, Transform& lookTransform)
	{
		if (currentState == EnemyState::DEAD) { return; }

		UpdateState(deltaTime);

	}

	


	void EnemyCharacter::TakeDamage(const int damage)
	{
		const float damagedHealth = GetCurrentHealth() - damage;

		if (damagedHealth <= 0)
		{
			m_health = 0;
			Die();
			return;
		}

		m_health = damagedHealth;

		m_counter = 0.0f;


	}
}