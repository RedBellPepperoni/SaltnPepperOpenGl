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
		m_targetPosition = target;
		m_currentPath = newpath;

		m_currentWaypointIndex = 1;
		m_isfollowingPath = true;
		m_targetClose = false;
	}

	void BuddyCharacter::OnUpdate(const float& deltaTime, Transform& buddyTransform, Transform& lookTransform)
	{
		const Vector3 currentPosition = buddyTransform.GetPosition() - Vector3{0.0f,0.81,0.0f};

		m_animator->OnUpdate(deltaTime);


		if (!m_isfollowingPath) { return; }

		if (!m_targetClose)
		{
			const Vector3 currentWaypoint = GetCurrentWayPoint();

			float waypointDistance = DistanceSquared(currentWaypoint, currentPosition);


			LOG_WARN("Buddy POs {0} {1} {2}", currentPosition.x, currentPosition.y, currentPosition.z);
			LOG_INFO("Waypint POs {0} {1} {2}", currentWaypoint.x, currentWaypoint.y, currentWaypoint.z);
			LOG_ERROR("Waypoint : {0}",waypointDistance);

			if (waypointDistance < 0.1f)
			{
				SetNextWaypoint();
			}

			else
			{
				const Vector3 direction = Normalize(currentWaypoint - currentPosition);
				Move(direction);
			}
		}

		else
		{
			const float distance = DistanceSquared(m_targetPosition, currentPosition);

			if (distance > 0.1f)
			{
				const Vector3 direction = Normalize(m_targetPosition - currentPosition);
				Move(direction);
			}

			else
			{
				m_isfollowingPath = false;
				m_targetClose = false;
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

	void BuddyCharacter::Move(const Vector3& targetDirection)
	{
		Vector3 finalVelocity = targetDirection * m_speed * m_forceMultiplier;
		float fallVelo = m_rigidBody->GetLinearVelocity().y;

		finalVelocity.y = fallVelo;
		m_rigidBody->SetLinearVelocity(finalVelocity);
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

	}

	void BuddyCharacter::Die()
	{

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
