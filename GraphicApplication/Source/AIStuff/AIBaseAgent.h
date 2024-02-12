#ifndef AIBASEAGENT_H
#define AIBASEAGENT_H
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	namespace Physics
	{
		class RigidBody3D;
	}

	using namespace Physics;
	using namespace Components;

	namespace AI
	{
		enum BehaviorState
		{	
			None = 0,
			Seek = 1,  // Continuously move towards the player
			Flee = 2,	// Continuesly move away from the player until a distance
			Pursue = 3, // Continuesly move towards a player until a set distance (if player goes near, do the Evade manuaer)
			Evade = 4,  // Flee from the player till a certain distance,
			Approach = 5 
		};

		enum AgentState
		{
			Idle,
			Moving,
			Interacting
		};

		class AIBaseAgent
		{
		protected:

			// Pointer to the physics body used to move the Agent
			RigidBody3D* m_rigidBody = nullptr;

			

			// The State in which the Agent is in
			AgentState m_currentState = AgentState::Moving;

			// The Behavior that the agent perfoms
			BehaviorState m_behavior = BehaviorState::None;

			float m_moveSpeed = 2.0f;
			float m_turnRate = 4.3f;

		protected:

			// Called when the state has reached its end
			void UpdateState();

			void MoveAgent(const Vector3& playerPosition, const Vector3& forwardDirection);
			void RotateAgent(const Vector3& playerPosition, const float deltaTime);

		public:

			// Pointer to the Display model for Visual Stuff
			Transform* m_lookTransform = nullptr;

		    AIBaseAgent();
			virtual ~AIBaseAgent();

			virtual void Init(RigidBody3D* RigidBodyRef, Transform* lookTransformRef);

			virtual void Update(float deltatime, const Vector3& playerPosition, const Vector3& playerDirection);

			virtual void SetBehaviour(BehaviorState newState) { m_behavior = newState; }
		};


	}
}

#endif // !AIBASEAGENT_H

