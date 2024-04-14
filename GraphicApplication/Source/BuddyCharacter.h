#ifndef BUDDYCHARACTER_H
#define BUDDYCHARACTER_H

#include "SaltnPepperEngine.h"
#include "IDamagable.h"

namespace SaltnPepperEngine
{
	class BuddyCharacter : public IDamagable
	{
	public:

		BuddyCharacter();
		virtual ~BuddyCharacter();

		virtual void TakeDamage(const int damage) override;

		void UpdateTargetandPath(const Vector3& target, const std::vector<Vector3>& newpath);

	private:

		void Move(const Vector3& targetDirection);
		void RotateModel(float deltatime, const Vector3& target, Transform& looktransform);
		void Attack(const Vector3& origin, const Vector3& target);

		void Die();

		void SetNextWaypoint();

		Vector3 GetCurrentWayPoint();

	private:


		Vector3 targetPosition;
		bool isfollowingPath = false;

		int currentWaypointIndex = 0;

		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;

	};
}

#endif // !BUDDYCHARACTER_H
