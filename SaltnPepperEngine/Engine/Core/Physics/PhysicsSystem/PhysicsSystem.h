#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
 #include <vector>


class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;



namespace SaltnPepperEngine
{
	namespace Physics
	{

		class RigidBody;

		struct PhysicsSystemData
		{
			using CollisionList = std::vector<std::pair<RigidBody*, RigidBody*>>;
			using CollisionSwapPair = std::pair<CollisionList, CollisionList>;


			UniquePtr<btCollisionConfiguration> CollisionConfiguration;
			UniquePtr<btDispatcher> Dispatcher;
			UniquePtr<btBroadphaseInterface> Broadphase;
			UniquePtr<btConstraintSolver> Solver;
			UniquePtr<btDiscreteDynamicsWorld> World;
			float simulationStep = 1.0f;

			CollisionSwapPair collisions;
		};


		class PhysicsSystem
		{
			

			inline static UniquePtr<PhysicsSystemData> data = nullptr;

		private:


			

		public:

			static void Init();
			static void OnUpdate(const float& deltaTime);

			static void PerformSimulationStep(float dt);
			static void SetSimulationStep(float timedelta);
			static float GetSimulationStep();

			static void InvoveCollisionUpdate();

			static void AddCollisionEntry(RigidBody* objectOne ,RigidBody* objectTwo);
			static PhysicsSystemData* GetCurrent();
			
		};

	}
}

#endif // !PHYSICSSYSTEM_H

