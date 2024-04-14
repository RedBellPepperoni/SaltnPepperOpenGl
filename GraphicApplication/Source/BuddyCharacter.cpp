#include "BuddyCharacter.h"

namespace SaltnPepperEngine
{
	BuddyCharacter::BuddyCharacter()
	{
	}
	BuddyCharacter::~BuddyCharacter()
	{
	}
	void BuddyCharacter::TakeDamage(const int damage)
	{
	}
	void BuddyCharacter::UpdateTargetandPath(const Vector3& target, const std::vector<Vector3>& newpath)
	{
	}
	void BuddyCharacter::Move(const Vector3& targetDirection)
	{
	}
	void BuddyCharacter::RotateModel(float deltatime, const Vector3& target, Transform& looktransform)
	{
	}
	void BuddyCharacter::Attack(const Vector3& origin, const Vector3& target)
	{
	}
	void BuddyCharacter::Die()
	{
	}
	void BuddyCharacter::SetNextWaypoint()
	{
	}
	Vector3 BuddyCharacter::GetCurrentWayPoint()
	{
		return Vector3();
	}
}
