#include "PhysicsSystem.h"
#include "Bullet3Bindings.h"
#include "PhysicsAPI.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Components/SceneComponents.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		

		void OnCollisionCallback();

		void PhysicsSystem::Init()
		{
			data = MakeUnique<PhysicsSystemData>();
			data->CollisionConfiguration = MakeUnique<btDefaultCollisionConfiguration>();
		
			data->Dispatcher = MakeUnique<btCollisionDispatcher>(data->CollisionConfiguration.get());
			data->Broadphase = MakeUnique<btDbvtBroadphase>();
			data->Solver = MakeUnique<btSequentialImpulseConstraintSolver>();
			data->Solver->reset();
			data->World = MakeUnique<btDiscreteDynamicsWorld>(data->Dispatcher.get(), data->Broadphase.get(), data->Solver.get(), data->CollisionConfiguration.get());
		
			data->World->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}


		void PhysicsSystem::OnUpdate(const float& deltaTime)
		{
			if (data->simulationStep != 0.0f)
			{
				PhysicsSystem::PerformSimulationStep(Min(deltaTime, data->simulationStep));
				OnCollisionCallback();
			}
		}

		void PhysicsSystem::PerformSimulationStep(float dt)
		{
			constexpr int maxSubSteps = 10;

			data->World->stepSimulation(dt, maxSubSteps);

		}

		void PhysicsSystem::SetSimulationStep(float timedelta)
		{
			data->simulationStep = timedelta;
		}

		float PhysicsSystem::GetSimulationStep()
		{
			return data->simulationStep;
		}

		void PhysicsSystem::InvoveCollisionUpdate()
		{
			auto& [currentCollisions, previousCollisions] = PhysicsSystem::GetCurrent()->collisions;

			std::sort(currentCollisions.begin(), currentCollisions.end());
			auto previousCollisionEntry = previousCollisions.begin();


			/// Work On Collision Stuff later

			constexpr auto CollisionCallback = [](RigidBody* objectOne, RigidBody* objectTwo, auto callbackMethod)
			{
				constexpr auto valid = [](RigidBody* object) -> bool
				{
					return object != nullptr;
				};

				//Call If both Objects are Valid

				if (valid(objectOne) && valid(objectTwo))
				{
					
					std::invoke(callbackMethod, objectOne, objectOne, objectTwo);

					std::invoke(callbackMethod, objectTwo, objectTwo, objectOne);
				}

					
			};

			for (auto it = currentCollisions.begin(); it != currentCollisions.end(); it++)
			{
				auto& [object1, object2] = *it;

				while (previousCollisionEntry != previousCollisions.end() && *previousCollisionEntry < *it)
				{
					auto& [obj1, obj2] = *previousCollisionEntry;
					CollisionCallback(obj1, obj2, &RigidBody::InvokeOnCollisionExitCallback);
					previousCollisionEntry++;
				}

				if (previousCollisionEntry == previousCollisions.end() || *previousCollisionEntry != *it)
					CollisionCallback(object1, object2, &RigidBody::InvokeOnCollisionEnterCallback);

				if (previousCollisionEntry != previousCollisions.end() && *previousCollisionEntry == *it)
					previousCollisionEntry++;

				CollisionCallback(object1, object2, &RigidBody::InvokeOnCollisionCallback);
			}

			while (previousCollisionEntry != previousCollisions.end())
			{
				auto& [obj1, obj2] = *previousCollisionEntry;
				CollisionCallback(obj1, obj2, &RigidBody::InvokeOnCollisionExitCallback);
				previousCollisionEntry++;
			}

			std::swap(currentCollisions, previousCollisions);
			currentCollisions.clear();
		
		}

		void PhysicsSystem::AddCollisionEntry(RigidBody* objectOne, RigidBody* objectTwo)
		{
			auto& [currentCollisions, previousCollisions] = PhysicsSystem::GetCurrent()->collisions;
			currentCollisions.emplace_back(objectOne, objectTwo);

		}

		PhysicsSystemData* PhysicsSystem::GetCurrent() 
		{ 
			return PhysicsSystem::data.get();
		}

		
	}
}