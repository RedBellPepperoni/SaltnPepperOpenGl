#ifndef ENEMYCHARACTER_H
#define ENEMYCHARACTER_H
#include "SaltnPepperEngine.h"
#include "IDamagable.h"

namespace SaltnPepperEngine
{
	struct EnemyLook
	{
		std::string look_tag = "EnemyLook";
	};

	class EnemyCharacter : public IDamagable
	{
		enum EnemyState
		{
			IDLE,
			WALKING,
			SHOOTING,
			RELOADING,
			TAKINGHIT,
			DYING,
			DEAD
		};


		float m_speed = 2.0f;

		const float m_forceMultiplier = 1.0f;

		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;
		Transform* m_weaponTransform = nullptr;

		float m_shootCooldown = 0.12f;
		float m_shootcounter = 0.0f;

		float m_counter = 0.0f;
		

		float m_ReloadTimer = 3.6f;
		float m_reloadCounter = 0.0f;

		bool isReloading = false;

		EnemyState currentState = EnemyState::IDLE;

		

	private:

		void UpdateState(const float deltaTime);

		void Move();
		
		void Die();

	public:


		EnemyCharacter();
		virtual ~EnemyCharacter() = default;

		void SetAnimatorRef(SkinnedAnimator* animRef);
		SkinnedAnimator* GetAnimator();
		void SetWeaponTransform(Transform* weaponTransform);
		void SetRigidBodyRef(RigidBody* bodyRef);

		void OnUpdate(float deltaTime,const Vector3& playerHeadPos, Transform& enemyTransform, Transform& lookTransform);

		virtual void TakeDamage(const int damage) override;
	};


	struct EnemyComponent
	{
		EnemyComponent();
		~EnemyComponent();

		EnemyCharacter* GetEnemy();


	private:
		SharedPtr<EnemyCharacter> enemyHandle = nullptr;
	};
}

#endif // !ENEMYCHARACTER_H
