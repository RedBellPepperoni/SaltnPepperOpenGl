#include "PhysicsSystem.h"
#include "Bullet3Bindings.h"
#include "PhysicsAPI.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Components/SceneComponents.h"
#include "RigidBody/RigidBody.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Scene/Scene.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		

		void OnCollisionCallback();

		void PhysicsSystem::Init()
		{
			/*data = MakeUnique<PhysicsSystemData>();
			data->CollisionConfiguration = MakeUnique<btDefaultCollisionConfiguration>();
		
			data->Dispatcher = MakeUnique<btCollisionDispatcher>(data->CollisionConfiguration.get());
			data->Broadphase = MakeUnique<btDbvtBroadphase>();
			data->Solver = MakeUnique<btSequentialImpulseConstraintSolver>();
			data->Solver->reset();
			data->World = MakeUnique<btDiscreteDynamicsWorld>(data->Dispatcher.get(), data->Broadphase.get(), data->Solver.get(), data->CollisionConfiguration.get());
		*/

			data = new PhysicsSystemData();
			data->CollisionConfiguration = new btDefaultCollisionConfiguration();

			data->Dispatcher = new btCollisionDispatcher(data->CollisionConfiguration);
			data->Broadphase = new btDbvtBroadphase();
			data->Solver = new btSequentialImpulseConstraintSolver();
			data->Solver->reset();
			data->World = new btDiscreteDynamicsWorld(data->Dispatcher, data->Broadphase, data->Solver, data->CollisionConfiguration);


			data->World->setGravity(btVector3(0.0f, -9.81f, 0.0f));
		}


		void PhysicsSystem::OnUpdate(const float& deltaTime)
		{

			ComponentView rigidView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<RigidBodyComponent>();
			//ComponentView boxColliderView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<BoxCollider>();
			//ComponentView sphereColliderView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<SphereCollider>();

			//if (rigidView.IsEmpty()) { return; }

			for (Entity rigidComp : rigidView)
			{

				Transform& transform = rigidComp.GetComponent<Transform>();
				RigidBody* body = rigidComp.GetComponent<RigidBodyComponent>().GetRigidBody();


				body->UpdateTransform(transform);

				BoxCollider* boxCol = rigidComp.TryGetComponent<BoxCollider>();
				if (boxCol) 
				{ 
					boxCol->DebugDraw(transform); 
					continue;
				}

				SphereCollider* sphereCol = rigidComp.TryGetComponent<SphereCollider>();
				if (sphereCol) 
				{ 
					sphereCol->DebugDraw(transform); 
					continue;
				}

				CapsuleCollider* capsuleCol = rigidComp.TryGetComponent<CapsuleCollider>();
				if (capsuleCol)
				{
					capsuleCol->DebugDraw(transform);
					continue;
				}

			}

			if (data->paused) { return; }


			if (data->simulationStep != 0.0f)
			{
				PhysicsSystem::PerformSimulationStep(Min(deltaTime, data->simulationStep));
				OnCollisionCallback();
			}

			//ComponentView rigidView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<RigidBodyComponent>();
			
			
		}

		void PhysicsSystem::Destroy()
		{
			free (data->World);
			free (data->Solver);
			free (data->Broadphase);
			free (data->Dispatcher);
			free (data->CollisionConfiguration);
			free (data);
		}

		void PhysicsSystem::PerformSimulationStep(float dt)
		{
			constexpr int maxSubSteps = 3;

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
			//auto& [currentCollisions, previousCollisions] = PhysicsSystem::GetCurrent()->collisions;

			////std::sort(currentCollisions.begin(), currentCollisions.end());
			//auto previousCollisionEntry = previousCollisions.begin();


			///// Work On Collision Stuff later

			//constexpr auto CollisionCallback = [](RigidBody* objectOne, RigidBody* objectTwo, auto callbackMethod)
			//{
			//	constexpr auto valid = [](RigidBody* object) -> bool
			//	{
			//		return object != nullptr;
			//	};

			//	//Call If both Objects are Valid

			//	if (valid(objectOne) && valid(objectTwo))
			//	{
			//		
			//		std::invoke(callbackMethod, objectOne, objectOne, objectTwo);

			//		std::invoke(callbackMethod, objectTwo, objectTwo, objectOne);
			//	}

			//		
			//};

			//for (auto it = currentCollisions.begin(); it != currentCollisions.end(); it++)
			//{
			//	auto& [object1, object2] = *it;

			//	while (previousCollisionEntry != previousCollisions.end() && *previousCollisionEntry < *it)
			//	{
			//		auto& [obj1, obj2] = *previousCollisionEntry;
			//		CollisionCallback(obj1, obj2, &RigidBody_Dep::InvokeOnCollisionExitCallback);
			//		previousCollisionEntry++;
			//	}

			//	if (previousCollisionEntry == previousCollisions.end() || *previousCollisionEntry != *it)
			//		CollisionCallback(object1, object2, &RigidBody_Dep::InvokeOnCollisionEnterCallback);

			//	if (previousCollisionEntry != previousCollisions.end() && *previousCollisionEntry == *it)
			//		previousCollisionEntry++;

			//	CollisionCallback(object1, object2, &RigidBody_Dep::InvokeOnCollisionCallback);
			//}

			//while (previousCollisionEntry != previousCollisions.end())
			//{
			//	auto& [obj1, obj2] = *previousCollisionEntry;
			//	CollisionCallback(obj1, obj2, &RigidBody_Dep::InvokeOnCollisionExitCallback);
			//	previousCollisionEntry++;
			//}

			//std::swap(currentCollisions, previousCollisions);
			//currentCollisions.clear();
		
		}

		void PhysicsSystem::AddCollisionEntry(RigidBody_Dep* objectOne, RigidBody_Dep* objectTwo)
		{
			auto& [currentCollisions, previousCollisions] = PhysicsSystem::GetCurrent()->collisions;
			currentCollisions.emplace_back(objectOne, objectTwo);

		}

		PhysicsSystemData* PhysicsSystem::GetCurrent() 
		{ 
			//return PhysicsSystem::data.get();
			return PhysicsSystem::data;
		}

		bool PhysicsSystem::GetPaused()
		{
			return PhysicsSystem::data->paused;
		}

		void PhysicsSystem::SetPaused(bool value)
		{
			PhysicsSystem::data->paused = value;
		}

		
	}
}