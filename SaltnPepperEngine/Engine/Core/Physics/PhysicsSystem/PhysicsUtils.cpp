#include "PhysicsUtils.h"
#include "PhysicsSystem.h"
#include "Bullet3Bindings.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Scene/Scene.h"
#include "BulletRigidBody.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

#define PHYSICSWORLD PhysicsSystem::GetCurrent()->World

		void OnCollisionCallback()
		{
			auto dispatcher = PHYSICSWORLD->getDispatcher(); 
			int numManiforlds = dispatcher->getNumManifolds(); 

			for (int i = 0; i < numManiforlds; i++)
			{
				btPersistentManifold* contactManifold = dispatcher->getManifoldByIndexInternal(i);
				const btCollisionObject* collider1 = contactManifold->getBody0();
				const btCollisionObject* collider2 = contactManifold->getBody1();

				if (!collider1->getCollisionShape()->isNonMoving() && !collider2->getCollisionShape()->isNonMoving())
				{
					//Entity object1 = Physics::GetRigidBodyParent(collider1);
					//Entity object2 = Physics::GetRigidBodyParent(collider2);

					//PhysicsSystem::AddCollisionEntry(object1, object2);
				}
			}
		}


		void PhysicsUtils::AddRigidBody(void* body)
		{

		}

		void PhysicsUtils::AddRigidBody(void* body, int group, int layer)
		{
		}
		void PhysicsUtils::RemoveRigidBody(void* body)
		{
		}
		void PhysicsUtils::ActiveRigidBodyIsland(void* body)
		{
		}

		void PhysicsUtils::SetRigidBodyEntity(BulletRigidBody* body, Entity entity)
		{



			entt::entity handle = entity.GetHandle();
			((btRigidBody*)body)->setUserPointer(reinterpret_cast<void*>(handle));
		}

		Entity PhysicsUtils::GetRigidBodyEntity(const void* body)
		{
			uint32_t handle = reinterpret_cast<uint32_t>(((btRigidBody*)body)->getUserPointer());

			return Entity();
		}
		
		Entity PhysicsUtils::RayCast(const Vector3& from, const Vector3& to)
		{
			return Entity();
		}
		Entity PhysicsUtils::RayCast(const Vector3& from, const Vector3& to, float& rayFraction)
		{
			return Entity();
		}
		Entity PhysicsUtils::RayCast(const Vector3& from, const Vector3& to, float& rayFraction, CollisionLayer::Mask rayCastMask)
		{
			return Entity();
		}
		Vector3 PhysicsUtils::GetGravity()
		{
			return Vector3();
		}
		void PhysicsUtils::SetGravity(const Vector3& gravity)
		{
		}
		void PhysicsUtils::PerformExtraSimulationStep(float timeDelta)
		{
		}
		void PhysicsUtils::SetSimulationStep(float timeDelta)
		{
		}
		float PhysicsUtils::GetSimulationStep()
		{
			return 0.0f;
		}
	}
}