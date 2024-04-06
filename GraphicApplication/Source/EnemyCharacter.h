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


	enum class ZombieType : uint8_t
	{
		WALK,
		CRAWL
	};

	class EnemyCharacter : public IDamagable
	{
		enum EnemyState
		{
			IDLE,
			WALKING,
			ATTACKING,
			TAKINGHIT,
			DEAD
		};


		enum EnemyBehaviour
		{
			DECIDING,
			WANDER,
			HUNT,
			ATTACK
			
		};

		enum SoundType
		{
			IDLESOUND,
			ATTACKSOUND,
			SCREAMSOUND,
			DEATHSOUND

		};


		float m_speed = 0.6f;

		const float m_forceMultiplier = 1.0f;

		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;
		Transform* m_weaponTransform = nullptr;

		const float m_shootCooldown = 0.12f;
		float m_shootcounter = 0.0f;

		float m_counter = 0.0f;
		float m_deathcounter = 0.0f;
		

		const float m_ReloadTimer = 3.6f;
		float m_reloadCounter = 0.0f;

		const float m_attackTimer = 1.2f;
		const float m_attackEventTimer = 2.0f;
		const float m_attackCooldown = 4.0f;
		float m_attackCounter = 0.0f;
		bool m_canAttack = true;
		bool isReloading = false;

		Audio::AudioSource* m_source = nullptr;
		Audio::AudioSource* m_idlesource = nullptr;

		EnemyState currentState = EnemyState::IDLE;
		EnemyBehaviour currentBehaviour = EnemyBehaviour::DECIDING;
		ZombieType currentType = ZombieType::WALK;
		SoundType currentSound = SoundType::IDLESOUND;

		const float m_detectionRadius = 5.0f;
		const float m_attackRadius = 1.5f;
		
		const float m_turnRate = 1.25f;

		bool m_playerDetected = false;
		bool m_playerInRange = false;
		bool m_playerInAttackRange = false;

		bool m_markedForDeath = false;

		Audio::AudioClip* m_idleclip = nullptr;
		Audio::AudioClip* m_alertclip = nullptr;
		Audio::AudioClip* m_attackclip = nullptr;
		Audio::AudioClip* m_deathclip = nullptr;

		bool m_canScream = false;
		bool m_shouldScream = true;
		float screamCounter = 0.0f;
		bool m_canattackScream = true;
	
		float attackSreamcounter = 0.0f;

	private:

		void UpdateState(const float deltaTime);

		void DetectPlayer(const float deltaTime, const Vector3& position,const Vector3& playerpos);
		void DecideMovement(const float deltatime,const Vector3& currentPosition, const Vector3& playerPosition, Transform& lookTrasform);
		void Move(const Vector3& targetDirection);
		void RotateModel(float deltatime, const Vector3& target, Transform& looktransform);
		void Attack(const Vector3& origin, const Vector3& target);

		void Die();

		void PlayIdleSound();
		void PlayScreamSound();
		void PlayAttackSound();
		void PlayDeathSound();

	public:


		EnemyCharacter();
		virtual ~EnemyCharacter() = default;

		void SetType(ZombieType type);

		void SetAnimatorRef(SkinnedAnimator* animRef);
		SkinnedAnimator* GetAnimator();
		void SetWeaponTransform(Transform* weaponTransform);
		void SetRigidBodyRef(RigidBody* bodyRef);
		void SetAudioSource(Audio::AudioSource* source, Audio::AudioSource* idleSource);
		void SetAudioClips(AudioClip* idle, AudioClip* alert, AudioClip* attack,AudioClip* death);

		void OnUpdate(float deltaTime,const Vector3& playerPos, Transform& enemyTransform, Transform& lookTransform);

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
