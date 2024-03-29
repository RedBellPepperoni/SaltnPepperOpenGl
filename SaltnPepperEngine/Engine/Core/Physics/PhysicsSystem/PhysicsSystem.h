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

//class btRigidBody;
//class btMotionState;

#include"Bullet3Bindings.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

		class RigidBody;
		class PhysicsSystem;

	


		struct PhysicsSystemData
		{
			using CollisionList = std::vector<std::pair<RigidBody*, RigidBody*>>;
			using CollisionSwapPair = std::pair<CollisionList, CollisionList>;


			//UniquePtr<btCollisionConfiguration> CollisionConfiguration;
			//UniquePtr<btDispatcher> Dispatcher;
			//UniquePtr<btBroadphaseInterface> Broadphase;
			//UniquePtr<btConstraintSolver> Solver;
			//UniquePtr<btDiscreteDynamicsWorld> World;

			btCollisionConfiguration* CollisionConfiguration;
			btDispatcher* Dispatcher;
			btBroadphaseInterface* Broadphase;
			btConstraintSolver* Solver;
			btDiscreteDynamicsWorld* World;
			float simulationStep = 1.0f;

			CollisionSwapPair collisions;
		};


		class PhysicsSystem
		{
			

			//inline static UniquePtr<PhysicsSystemData> data = nullptr;
			

		private:


			

		public:

			inline static PhysicsSystemData* data = nullptr;

			static void Init();
			static void OnUpdate(const float& deltaTime);

			static void Destroy();

			static void PerformSimulationStep(float dt);
			static void SetSimulationStep(float timedelta);
			static float GetSimulationStep();

			static void InvoveCollisionUpdate();

			static void AddCollisionEntry(RigidBody* objectOne ,RigidBody* objectTwo);
			static PhysicsSystemData* GetCurrent();
			
		};

		class RShape
		{
		private:
			btSphereShape* collider = nullptr;

		public:
			RShape() { collider = new btSphereShape(1); }
			~RShape() { delete collider; }

			btSphereShape* GetShape() {
				return collider;
			}
		};

		class RBody
		{
		private:

			btRigidBody* bodyHandle = nullptr;
			btMotionState* motionHandle = nullptr;

		public:

			RBody(const Vector3& position, btCollisionShape* shape)
			{
				btVector3 pos = ToBulletVector3(position);

				motionHandle = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
					pos));

				btRigidBody::btRigidBodyConstructionInfo info(10, motionHandle, shape, btVector3(0, 0, 0));
				bodyHandle = new btRigidBody(info);

				PhysicsSystem::GetCurrent()->World->addRigidBody(bodyHandle);
			}
			~RBody()
			{
				delete motionHandle;
				delete bodyHandle;
			};

			btRigidBody* GetBody() { return bodyHandle; }

		};

	}
}

#endif // !PHYSICSSYSTEM_H

