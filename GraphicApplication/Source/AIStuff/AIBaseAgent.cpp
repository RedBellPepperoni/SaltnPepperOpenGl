#include "AIBaseAgent.h"
#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Utils/Time/Time.h"
#include "Engine/Utils/Maths/Random.h"

namespace SaltnPepperEngine
{
	namespace AI
	{
		
			
		

		void AIBaseAgent::UpdateState()
		{
		}

		void AIBaseAgent::MoveAgent(const Vector3& playerPosition, const Vector3& forwardDirection, const float deltaTime)
		{
			Vector3 ProjectedPosition = playerPosition + forwardDirection * 12.0f;
			
			// get the Direction vector
			Vector3 MoveDirection = Vector3(0.0f);
			Vector3 position = m_rigidBody->GetPosition();
			Vector3 forward = Normalize(m_lookTransform->GetForwardVector());
			float distance = 0;

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
				
				 distance = DistanceSquared(position, playerPosition);

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


			case SaltnPepperEngine::AI::Wander:

				// Get the squared distance from the taget point
				distance = DistanceSquared(position, targetWanderPos);

				// We are close Enough , Do the decision thing
				if (distance < 0.5f)
				{
					// Stop the Agent
					MoveDirection = Vector3(0.0f);
					m_rigidBody->SetVelocity(Vector3(0.0f));

					currentTime += deltaTime;

					// Finally Dont with the desition
					if (currentTime < m_decisionTime) { return; }
					
					currentTime = 0.0f;

					Vector3 PrimaryWanderPosition(position + forward * m_primaryWanderRadius);

					//float theta = Random32::Range.GetRandom(0.0f, 2 * PI);
					int randomIndex = Random32::Range.GetRandom(0,secondaryAngleList.size()-1);
					float theta = secondaryAngleList[randomIndex];

					//LOG_WARN("Angle : {0}",theta * RADtoDEG);

					Vector3 SecondaryWanderPosition(m_secondaryWanderRadius * Cos(theta), 0.0f, m_secondaryWanderRadius * Sin(theta));

					targetWanderPos = PrimaryWanderPosition + SecondaryWanderPosition;



				}

				MoveDirection = Normalize(targetWanderPos - position);
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


			if (m_behavior == BehaviorState::Wander)
			{
				MoveAgent(targetWanderPos, Vector3(0.0f,0.0f,0.0f), deltatime);
			}

			else
			{
				MoveAgent(playerPosition, playerDirection, deltatime);
			}

			if (m_behavior == BehaviorState::Seek || m_behavior == BehaviorState::Pursue || m_behavior == BehaviorState::Approach)
			{
				RotateAgent(playerPosition, deltatime);
			}

			else if (m_behavior == BehaviorState::Wander)
			{
				RotateAgent(targetWanderPos, deltatime);
			}

			else
			{
				RotateAgent(playerPosition + (playerDirection + 2.0f),deltatime);
			}


		}

		void AIBaseAgent::SetBehaviour(BehaviorState newState)
		{
			 m_behavior = newState; 

			 if (m_behavior == AI::Wander)
			 {
				 Vector3 position = m_rigidBody->GetPosition();
				 Vector3 forward = Normalize(m_lookTransform->GetForwardVector());

				 Vector3 PrimaryWanderPosition(position + forward * m_primaryWanderRadius);




				 float theta = Random32::Range.GetRandom(0.0f , 2*PI);

				 Vector3 SecondaryWanderPosition(m_secondaryWanderRadius * Cos(theta), 0.0f, m_secondaryWanderRadius * Sin(theta));

				 targetWanderPos = PrimaryWanderPosition + SecondaryWanderPosition;
			 }
		}

		void AIBaseAgent::SetWanderParams(float primaryRadius, float secondaryRadius, float decisionTime, float turnRate)
		{
			m_primaryWanderRadius = primaryRadius;
			m_secondaryWanderRadius = secondaryRadius;

			m_decisionTime = decisionTime;
			m_turnRate = turnRate;

		}
	}
}