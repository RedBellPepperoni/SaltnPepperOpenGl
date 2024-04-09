#ifndef AIBASEAGENT_H
#define AIBASEAGENT_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <vector>
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
			Approach = 5 ,
			Wander = 6,
			PathFollow = 7
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



			const float m_outerPathRadius = 10.0f;
			const float m_innerPathRadius = 8.0f;
			Vector3 m_waypointCenter = Vector3(0.0f);;
			std::vector<Vector3> m_followPathWaypoints = std::vector<Vector3>();
			int m_currentWaypointIndex = 0;


			float m_primaryWanderRadius = 5.0f;
			float m_secondaryWanderRadius = 2.0f;
			float m_decisionTime = 2.0f;
			float currentTime = 0.0f;

			Vector3 targetWanderPos = Vector3(0.0f);



			std::vector<float> secondaryAngleList =
			{
				PI / 6,
				PI / 3,
				PI / 2,
				3 * PI / 4,
				PI,
				5 * PI / 4,
				3 * PI / 2,
				7 * PI / 4,
				0
			};

	

		protected:



			void SetNextWayPoint();
			const Vector3 GetCurrentWayPoint() const;
			// Called when the state has reached its end
			void UpdateState();

			void MoveAgent(const Vector3& playerPosition, const Vector3& forwardDirection, const float deltaTime);
			void RotateAgent(const Vector3& playerPosition, const float deltaTime);

		public:

			// Pointer to the Display model for Visual Stuff
			Transform* m_lookTransform = nullptr;

		    AIBaseAgent();
			virtual ~AIBaseAgent();

			void SetWaypointCenter(const Vector3& center);
			void SetWaypoints(const std::vector<Vector3>& points);

			virtual void Init(RigidBody3D* RigidBodyRef, Transform* lookTransformRef);

			virtual void Update(float deltatime, const Vector3& playerPosition, const Vector3& playerDirection);

			virtual void SetBehaviour(BehaviorState newState);

			virtual void SetWanderParams(float primaryRadius, float secondaryRadius, float decisionTime, float turnRate = 1.0f);

		};



	}


	struct AIAgentComponent
	{
		AIAgentComponent();
		~AIAgentComponent();

		AI::AIBaseAgent* GetAgent();

	private:

		SharedPtr<AI::AIBaseAgent> m_handle = nullptr;

	};
}

#endif // !AIBASEAGENT_H

