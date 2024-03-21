#include "PhysicsUtils.h"
#include "PhysicsSystem.h"
#include "Bullet3Bindings.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Scene/Scene.h"
#include "Engine/Core/Physics/PhysicsSystem/RigidBody/BulletRigidBody.h"

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
				const btCollisionObject* colliderOne = contactManifold->getBody0();
				const btCollisionObject* colliderTwo = contactManifold->getBody1();

				if (!colliderOne->getCollisionShape()->isNonMoving() && !colliderTwo->getCollisionShape()->isNonMoving())
				{
					RigidBody* objectOne = PhysicsUtils::GetRigidBodyParent(colliderOne);
					RigidBody* objectTwo = PhysicsUtils::GetRigidBodyParent(colliderTwo);

					PhysicsSystem::AddCollisionEntry(objectOne, objectTwo);
				}
			}
		}

		struct CustomRayCastCallback : public btCollisionWorld::ClosestRayResultCallback
		{
			CustomRayCastCallback(const btVector3& from, const btVector3& to, CollisionLayer::Mask rayCastMask)
				: btCollisionWorld::ClosestRayResultCallback(from, to)
			{
				this->m_collisionFilterGroup = CollisionGroup::ALL;
				this->m_collisionFilterMask = rayCastMask;
			}

			RigidBody* GetResult() const
			{
				if (!this->hasHit()) return nullptr;
				return PhysicsUtils::GetRigidBodyParent(this->m_collisionObject);
			}

			float GetRayFraction() const
			{
				return this->m_closestHitFraction;
			}
		};


		void PhysicsUtils::AddRigidBody(void* body)
		{
			PHYSICSWORLD->addRigidBody((btRigidBody*)body);
		}

		void PhysicsUtils::AddRigidBody(void* body, int group, int layer)
		{
			PHYSICSWORLD->addRigidBody((btRigidBody*)body, group, layer);
		}

		void PhysicsUtils::RemoveRigidBody(void* body)
		{
			PHYSICSWORLD->removeRigidBody((btRigidBody*)body);
		}

		void PhysicsUtils::ActiveRigidBodyIsland(void* body)
		{
			int islandTag = ((btRigidBody*)body)->getIslandTag();
			int numberOfObjects = PHYSICSWORLD->getNumCollisionObjects();
			auto& objectArray = PHYSICSWORLD->getCollisionObjectArray();
			for (int i = 0; i < numberOfObjects; i++)
			{
				if (objectArray[i]->getIslandTag() == islandTag)
					objectArray[i]->setActivationState(ACTIVE_TAG);
			}
		}

		void PhysicsUtils::SetRigidBodyParent(BulletRigidBody* body, RigidBody* entity)
		{
			((btRigidBody*)body)->setUserPointer(reinterpret_cast<void*>(entity));
		}

		RigidBody* PhysicsUtils::GetRigidBodyParent(const void* body)
		{
			RigidBody* returnBody = reinterpret_cast<RigidBody*>(((btRigidBody*)body)->getUserPointer());
			return returnBody;
		}

		RigidBody* PhysicsUtils::RayCast(const Vector3& from, const Vector3& to)
		{
			float rayFraction = 0.0f;
			return PhysicsUtils::RayCast(from, to, rayFraction);
		}

		RigidBody* PhysicsUtils::RayCast(const Vector3& from, const Vector3& to, float& rayFraction)
		{
			return PhysicsUtils::RayCast(from, to, rayFraction, CollisionLayer::RAYCAST_ONLY);
		}

		RigidBody* PhysicsUtils::RayCast(const Vector3& from, const Vector3& to, float& rayFraction, CollisionLayer::Mask rayCastMask)
		{
			btVector3 bulletFrom = ToBulletVector3(from);
			btVector3 bulletTo = ToBulletVector3(to);

			CustomRayCastCallback callback(bulletFrom, bulletTo, rayCastMask);

			PHYSICSWORLD->rayTest(bulletFrom, bulletTo, callback);
			rayFraction = callback.GetRayFraction();

			return callback.GetResult();
		}

		

		
		
		Vector3 PhysicsUtils::GetGravity()
		{
			return  FromBulletVector3(PHYSICSWORLD->getGravity());
		}

		void PhysicsUtils::SetGravity(const Vector3& gravity)
		{
			PHYSICSWORLD->setGravity(ToBulletVector3(gravity));
		}

		void PhysicsUtils::PerformExtraSimulationStep(float timeDelta)
		{
			PhysicsSystem::PerformSimulationStep(timeDelta);
		}

		void PhysicsUtils::SetSimulationStep(float timeDelta)
		{
			PhysicsSystem::SetSimulationStep(timeDelta);
		}

		float PhysicsUtils::GetSimulationStep()
		{
			return PhysicsSystem::GetSimulationStep();
		}
	}
}