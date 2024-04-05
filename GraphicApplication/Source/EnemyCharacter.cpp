#include "EnemyCharacter.h"
namespace SaltnPepperEngine
{
	void EnemyCharacter::UpdateState(const float deltaTime)
	{
		float duration = 0.0f;

		


		switch (currentState)
		{
		case SaltnPepperEngine::EnemyCharacter::IDLE:

			m_animator->PlayAnimationbyName("Idle");
			m_animator->SetTransitiontime(0.1f);

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

			m_animator->PlayAnimationbyName("HitOne",false);

			m_counter += deltaTime;
			duration = 0.2f;//m_animator->GetAnimationByName("HitOne")->GetDuration();

			if (m_counter > duration)
			{
				currentState = EnemyState::IDLE;
			}

			break;

		case SaltnPepperEngine::EnemyCharacter::DYING:

		
			m_counter += deltaTime;
			duration = 3.50f;

			if (m_counter > duration)
			{
				currentState = EnemyState::DEAD;
			}

			break;
		}
	}

	void EnemyCharacter::Move()
	{
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

	void EnemyCharacter::OnUpdate(float deltaTime, const Vector3& playerHeadPos, Transform& enemyTransform, Transform& lookTransform)
	{
		if (currentState == EnemyState::DEAD) { return; }

		UpdateState(deltaTime);

		//Bone* handbone = m_animator->GetCurrentAnimation()->FindBone("R_wrist");

		
		Matrix4 wristRTransform = m_animator->GetFinalBoneMatrices()[41];

		if (m_weaponTransform != nullptr)
		{
			Matrix4 newMat = lookTransform.GetMatrix() * (wristRTransform);
			m_weaponTransform->SetLocalMatrix(newMat);
		}

		m_animator->OnUpdate(deltaTime);

		
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

		m_animator->SetTransitiontime(0.01f);
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