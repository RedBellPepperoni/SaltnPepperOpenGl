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

		

			/// Work On Collision Stuff later

		/*	std::sort(currentCollisions.begin(), currentCollisions.end());
			auto previousCollisionEntry = previousCollisions.begin();

			constexpr auto CollisionCallback = [](Entity object1, Entity object2, auto callbackMethod)
				{
					constexpr auto valid = [](Entity object) -> bool
						{
							return object.IsValid() && object.TryGetComponent<RigidBodyComponent>() != nullptr;
						};

					if (valid(object1) && valid(object2))
						std::invoke(callbackMethod, object1.GetComponent<RigidBodyComponent>(), object1, object2);
					if (valid(object2) && valid(object1))
						std::invoke(callbackMethod, object2.GetComponent<RigidBodyComponent>(), object2, object1);
				};

			*/
		
		}

		PhysicsSystemData* PhysicsSystem::GetCurrent()
		{
			return PhysicsSystem::data.get();
		}

		
	}
}