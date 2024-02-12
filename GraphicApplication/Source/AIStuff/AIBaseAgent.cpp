#include "AIBaseAgent.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/Transform.h"

namespace SaltnPepperEngine
{
	namespace AI
	{
		
			

		void AIBaseAgent::UpdateState()
		{
			 
		}

		void AIBaseAgent::MoveAgent(const Vector3& playerPosition, const Vector3& forwardDirection)
		{
			Vector3 ProjectedPosition = playerPosition + forwardDirection * 12.0f;
			
			// get the Direction vector
			Vector3 MoveDirection = Vector3(0.0f);
			Vector3 position = m_rigidBody->GetPosition();

			switch (m_behavior)
			{
			case SaltnPepperEngine::AI::Pursue: 
						
				if (DistanceSquared(position, ProjectedPosition) > 49.0f)
				{
					MoveDirection = Normalize(ProjectedPosition - position);
				}
				else
				{
					MoveDirection = Normalize(playerPosition - position);
				}

				break;

			case SaltnPepperEngine::AI::Seek: MoveDirection = Normalize(playerPosition - position);
				break;

			case SaltnPepperEngine::AI::Evade: 
				
				
				if (DistanceSquared(position, ProjectedPosition) < 200.0f)
				{
					MoveDirection = Normalize(position - ProjectedPosition);
				}

				break;

			case SaltnPepperEngine::AI::Flee:  
				if (DistanceSquared(position, playerPosition) < 150.0f)
				{
					MoveDirection = Normalize(position - playerPosition);
				}
				break;


			case SaltnPepperEngine::AI::Approach:
				
				float distance = DistanceSquared(position, playerPosition);

				if ( distance> 55.0f)
				{
					MoveDirection = Normalize(playerPosition - position);
				}
				else if(distance < 30.0f)
				{
					MoveDirection = Normalize(position - playerPosition);
				}
				else
				{
					MoveDirection = Vector3(0.0f);
				}


				break;
			}

			m_rigidBody->SetForce(MoveDirection * 50.0f * m_moveSpeed);
		}

		void AIBaseAgent::RotateAgent(const Vector3& playerPosition,const float deltaTime)
		{

			float rotationStep = m_turnRate * deltaTime;
			Vector3 targetDirection = Normalize(playerPosition - m_rigidBody->GetPosition());
			Vector3 forwardDirection = Normalize(m_lookTransform->GetForwardVector());
			float angle = glm::angle(targetDirection, forwardDirection);

			if (angle < rotationStep) { return; }

			Vector3 rotationAxis = Cross(forwardDirection, targetDirection);

			if(rotationAxis.y > 0.01f)
			{
				rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
			}
			else
			{
				rotationAxis = Vector3(0.0f, -1.0f, 0.0f);
			}

			

			switch (m_behavior)
			{
			//case SaltnPepperEngine::AI::Pursue: // Fall down condition
			//case SaltnPepperEngine::AI::Seek: ;
			//	break;
			case SaltnPepperEngine::AI::Evade: // Fall down condition
			case SaltnPepperEngine::AI::Flee: rotationStep = -rotationStep;
				break;

			case SaltnPepperEngine::AI::Approach:
				break;
			default:
				break;
			}

			Quaternion rotation = GetQuaternion(Rotate(m_lookTransform->GetLocalMatrix(), rotationStep, rotationAxis));
			m_lookTransform->SetRotation(rotation);
		}

	


		AIBaseAgent::AIBaseAgent()
		{

		}

		AIBaseAgent::~AIBaseAgent()
		{


		}

		void AIBaseAgent::Init(RigidBody3D* RigidBodyRef, Transform* lookTransformRef)
		{
			m_rigidBody = RigidBodyRef;
			m_lookTransform = lookTransformRef;
		}

		void AIBaseAgent::Update(float deltatime, const Vector3& playerPosition, const Vector3& playerDirection)
		{	
			if (!m_rigidBody || !m_lookTransform) { return; }
			
			if (m_behavior == BehaviorState::None) { return; }

			// Check if the state needs to change
			UpdateState();

			if (m_currentState == AgentState::Idle) { return; }

			MoveAgent(playerPosition, playerDirection);

			if (m_behavior == BehaviorState::Seek || m_behavior == BehaviorState::Pursue || m_behavior == BehaviorState::Approach)
			{
				RotateAgent(playerPosition, deltatime);
			}

			else
			{
				RotateAgent(playerPosition + (playerDirection + 2.0f),deltatime);
			}
		}
	}
}