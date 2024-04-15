#ifndef BUDDYCHARACTER_H
#define BUDDYCHARACTER_H

#include "SaltnPepperEngine.h"
#include "IDamagable.h"

namespace SaltnPepperEngine
{
	class GameManager;

	struct BuddyLook
	{
		std::string look_tag = "BuddyLook";
	};

	class BuddyCharacter : public IDamagable
	{

	public:

		enum BuddyState
		{
			IDLE,
			WALKING,
			ATTACKING,
			TAKINGHIT,
			DEAD
		};


		enum BuddyBehaviour
		{
			DECIDING,
			MOVING,
			ATTACK

		};

		enum BuddySound
		{
			IDLESOUND,
			ATTACKSOUND,
			SCREAMSOUND,
			DEATHSOUND

		};

	public:

		BuddyCharacter();
		virtual ~BuddyCharacter();

		virtual void TakeDamage(const int damage) override;
		
		void UpdateTargetandPath(const Vector3& target, const std::vector<Vector3>& newpath);

		void OnUpdate(const float& deltaTime, Transform& buddyTransform, Transform& lookTransform);


		void SetAnimatorRef(SkinnedAnimator* animRef);
		SkinnedAnimator* GetAnimator();
		void SetRigidBodyRef(RigidBody* bodyRef);
		void SetGameManagerRef(GameManager* finder);

	private:

		void UpdateAnimState(const float deltaTime);

		void Move(const Vector3& targetDirection);
		void RotateModel(float deltatime, const Vector3& target, Transform& looktransform);
		void Attack(const Vector3& origin, const Vector3& target);

		void Die();

		void SetNextWaypoint();

		Vector3 GetCurrentWayPoint();

	private:

		const float m_detectionRadius = 5.0f;
		const float m_attackRadius = 1.5f;

		const float m_turnRate = 2.25f;
		float m_speed = 1.6f;
		const float m_forceMultiplier = 1.0f;

		Vector3 m_targetPosition = Vector3{0.0f};
		bool m_isfollowingPath = false;
		bool m_targetClose = false;

		int m_currentWaypointIndex = 0;

		GameManager* m_gameManagerRef = nullptr;
		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;

		std::vector<Vector3> m_currentPath = std::vector<Vector3>{};


		float m_animCounter = 0.0f;
		const float animationFrameRate = 1.0f / 30.0f;

		BuddyState currentState = BuddyState::IDLE;
		BuddyBehaviour currentBehaviour = BuddyBehaviour::DECIDING;

	};

	struct BuddyComponent
	{
		BuddyComponent();
		~BuddyComponent();

		BuddyCharacter* GetBuddy();
	private:

		SharedPtr<BuddyCharacter> m_buddyHandle = nullptr;
	};
}

#endif // !BUDDYCHARACTER_H
