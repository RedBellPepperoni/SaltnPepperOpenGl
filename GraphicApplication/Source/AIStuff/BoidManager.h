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

			uint32_t flockId = 0;

			Vector2 velocity;
			Vector2 targetVec;
			Vector2 avoidanceVec;
			Vector2 cohesionVec;
			Vector2 alignmentVec;
			Vector2 obstacleAvoidanceVec;
			Vector2 predatorAvoidanceVec;

			inline const void ProcessVelocity()
			{
				velocity = velocity + targetVec + avoidanceVec + alignmentVec + cohesionVec + obstacleAvoidanceVec + predatorAvoidanceVec;
			};

		};

		struct Obstacle
		{
			std::string name = "Obstacle_Tag";
		};

		struct Predator
		{
			std::string name = "Predator_Tag";
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

			void SeekAI(const Vector2& position, Flock& flockRef, Vector2 target);
			void SpreadAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView);
			void AlignAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView);
			void GroupAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView);
			void UpdateAIRotation(uint32_t index);



			void UpdateFlock();
			void CheckObstacles(const Vector2& position, Flock& flockRef,  const std::vector<Vector2>& obstaclePositionList);
			void CheckPredator(const Vector2& position, Flock& flockRef, const Vector2& predatorPosition);

			
			void PredatorSeek();
			void PredatorCheckObtacles();
			void UpdatePredator();
			void PredatorUpdateRotation();

		private:

			// ======= Variables ============

			size_t flockSize = 0;
			size_t obstacleCount = 0;

			//std::vector<RigidBody3D*> steeringObjects;

			float localdeltaTime = 0.0f;
			Vector2 Bounds = Vector2( 160,90 );

			/*RigidBody3D* Attractor;
			RigidBody3D* Predator;*/

			// Boids Stuff
			/*std::vector<Vector2> velocity;
			std::vector<Vector2> targetVec;
			std::vector<Vector2> avoidanceVec;
			std::vector<Vector2> cohesionVec;
			std::vector<Vector2> alignmentVec;
			std::vector<Vector2> obstacleAvoidanceVec;
			std::vector<Vector2> predatorAvoidanceVec;*/

			// PredatorStuff
			Vector2 pVelocity;
			Vector2 pTargetVec;
			Vector2 pObstacleAvoidanceVec;


			float obstacleRadius = 2.0f;
			float avoidanceRadius = 16.0f;
			float predatorAvoidanceRadius = 12.0f;

			float avoidanceLBoundary = 180.0f - 15.0f;
			float avoidanceHBoundary = 180.0f + 15.0f;

			float fovLBoundary = 180.0f - 30.0f;
			float fovHBoundary = 180.0f + 30.0f;

			float attractorSpeed = 3.0f;

		};
	}

}

#endif // !BOINDMANAGER_H


