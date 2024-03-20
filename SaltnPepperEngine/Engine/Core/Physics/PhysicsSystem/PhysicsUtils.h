#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

#include "Engine/Core/EntitySystem/Entity.h"
#include "PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

        class BulletRigidBody;

		class PhysicsUtils
		{
		public:

			static void AddRigidBody(void* body);
			static void AddRigidBody(void* body, int group, int layer);

            static void RemoveRigidBody(void* body);
            static void ActiveRigidBodyIsland(void* body);

            static void SetRigidBodyEntity(BulletRigidBody* body, Entity entity);
            
            static Entity GetRigidBodyEntity(const void* body);

            static Entity RayCast(const Vector3& from, const Vector3& to);
            static Entity RayCast(const Vector3& from, const Vector3& to, float& rayFraction);
            static Entity RayCast(const Vector3& from, const Vector3& to, float& rayFraction, CollisionLayer::Mask rayCastMask);
            static Vector3 GetGravity();

            static void SetGravity(const Vector3& gravity);
            static void PerformExtraSimulationStep(float timeDelta);
            static void SetSimulationStep(float timeDelta);
            static float GetSimulationStep();

		};

	}
}

#endif // !PHYSICSUTILS_H
