#include "BuddyCharacter.h"

#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"
namespace SaltnPepperEngine
{
	BuddyCharacter::BuddyCharacter()
	{
		m_entityManger = Application::GetCurrent().GetCurrentScene()->GetEntityManager();
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

		
		UpdateAnimState(deltaTime);

		m_animCounter += deltaTime;

		if (m_animCounter >= animationFrameRate)
		{
			m_animCounter = 0.0f;
			m_animator->OnUpdate(animationFrameRate);


		}



		if (!m_isfollowingPath) { return; }

		if (!m_targetClose)
		{
			const Vector3 currentWaypoint = GetCurrentWayPoint();

			float waypointDistance = DistanceSquared(currentWaypoint, currentPosition);

			if (waypointDistance < 0.1f)
			{
				SetNextWaypoint();
			}

			else
			{
				const Vector3 direction = Normalize(currentWaypoint - currentPosition);
				Move(direction);
				RotateModel(deltaTime,direction, lookTransform);
			}
		}

		else
		{
			const float distance = DistanceSquared(m_targetPosition, currentPosition);

			if (distance > 0.1f)
			{
				const Vector3 direction = Normalize(m_targetPosition - currentPosition);
				Move(direction);
				RotateModel(deltaTime, direction, lookTransform);

			}

			else
			{
				m_isfollowingPath = false;
				m_targetClose = false;
				currentState = BuddyState::IDLE;
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

	void BuddyCharacter::UpdateAnimState(const float deltaTime)
	{
		float duration = 0.0f;



		switch (currentState)
		{
		case BuddyState::IDLE:

			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("IdleUnarmed");



			break;

		case BuddyState::WALKING:
			m_animator->SetTransitiontime(0.4f);
			m_animator->PlayAnimationbyName("WalkUnarmed");

			//PlayIdleSound();

			break;

		case BuddyState::ATTACKING:
			/*m_animator->SetTransitiontime(0.2f);
			m_animator->PlayAnimationbyName("Attack");

			m_counter += deltaTime;
			duration = 1.0f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = BuddyState::IDLE;
			}

			break;*/


		case BuddyState::TAKINGHIT:

			/*m_animator->SetTransitiontime(0.05f);
			m_animator->PlayAnimationbyName("HitOne", false);

			m_counter += deltaTime;
			duration = 0.6f;

			if (m_counter > duration)
			{
				m_counter = 0.0f;
				currentState = BuddyState::IDLE;
			}*/

			break;

		}
	}
		

	void BuddyCharacter::Move(const Vector3& targetDirection)
	{
		Vector3 finalVelocity = targetDirection * m_speed * m_forceMultiplier;
		float fallVelo = m_rigidBody->GetLinearVelocity().y;

		finalVelocity.y = fallVelo;
		m_rigidBody->SetLinearVelocity(finalVelocity);

		currentState = BuddyState::WALKING;
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
