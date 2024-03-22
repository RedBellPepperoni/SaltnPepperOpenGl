#ifndef BOINDMANAGER_H
#define BOINDMANAGER_H

//#include "Engine/Utils/Maths/MathDefinitions.h"
//#include "Engine/Core/Physics/PhysicsEngine/RigidBody3D.h"

/// Using the Entire Include Directiory for now
#include "SaltnPepperEngine.h"

#include <vector>

namespace SaltnPepperEngine
{
	using Physics::RigidBody3D;

	namespace AI
	{
		struct Flock
		{
			std::string name = "Flock_Tag";
		};

		struct Obstacle
		{
			std::string name = "Obstacle_Tag";
		};

		class BoidManager
		{
		public:

			BoidManager() = default;
			void OnInit();
			void OnUpdate(const float& deltaTime);




			void CreateFlockEntity(const Vector3& position);
			void CreateObstacle(const Vector3& position);
			void CreatePredator(const Vector3& position);
			
		private:

			void SeekAI(uint32_t index, Vector2 target);
			void SpreadAI(uint32_t index);
			void AlignAI(uint32_t index);
			void GroupAI(uint32_t index);
			void UpdateAIRotation(uint32_t index);



			void UpdateFlock();
			bool CheckAvoidanceFOV(uint32_t i, uint32_t j);
			bool CheckFOV(uint32_t i, uint32_t j);
			void CheckObtacles(uint32_t index);
			void CheckPredator(uint32_t index);

			void AddObstacle(float x, float y);
			void AddOSteeringObject(float x, float y);
			void MoveAttractor(const Vector3& position);
			void ClearObjects();

			void PredatorSeek();
			void PredatorCheckObtacles();
			void UpdatePredator();
			void PredatorUpdateRotation();

		private:

			// ======= Variables ============

			size_t flockSize = 0;
			size_t obstacleCount = 0;

			//std::vector<RigidBody3D*> steeringObjects;


			Vector2 Bounds = Vector2( 160,90 );

			RigidBody3D* Attractor;
			RigidBody3D* Predator;

			// Boids Stuff
			std::vector<Vector2> velocity;
			std::vector<Vector2> targetVec;
			std::vector<Vector2> avoidanceVec;
			std::vector<Vector2> cohesionVec;
			std::vector<Vector2> alignmentVec;
			std::vector<Vector2> obstacleAvoidanceVec;
			std::vector<Vector2> predatorAvoidanceVec;

			// PredatorStuff
			Vector2 pVelocity;
			Vector2 pTargetVec;
			Vector2 pObstacleAvoidanceVec;


			float obstacleRadius = 2.0f;
			float avoidanceRadius = 5.0f;
			float predatorAvoidanceRadius = 12.0f;

			float avoidanceLBoundary = 180.0f - 15.0f;
			float avoidanceHBoundary = 180.0f + 15.0f;

			float fovLBoundary = 180.0f - 30.0f;
			float fovHBoundary = 180.0f + 30.0f;

			bool enableFOV = true;

			float attractorSpeed = 3.0f;

		};
	}

}

#endif // !BOINDMANAGER_H


