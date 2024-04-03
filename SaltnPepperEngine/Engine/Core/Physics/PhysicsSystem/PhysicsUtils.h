#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

#include "Engine/Core/EntitySystem/Entity.h"
#include "PhysicsAPI.h"
#include "RigidBody/RigidBody_Dep.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

        class BulletRigidBody;

		class PhysicsUtils
		{
		public:

			static void AddRigidBody(btRigidBody* body);
			static void AddRigidBody(btRigidBody* body, int group, int layer);
			
            static void RemoveRigidBody(btRigidBody* body);
            static void ActiveRigidBodyIsland(btRigidBody* body);

            static void SetRigidBodyParent(btRigidBody* body, RigidBody* entity);
            
            static RigidBody* GetRigidBodyParent(const void* body);

            static RigidBody* RayCast(const Vector3& from, const Vector3& to);
            static RigidBody* RayCast(const Vector3& from, const Vector3& to, float& rayFraction);
            static RigidBody* RayCast(const Vector3& from, const Vector3& to, float& rayFraction, CollisionMask::Mask rayCastMask);
            static Vector3 GetGravity();

            static void SetGravity(const Vector3& gravity);
            static void PerformExtraSimulationStep(float timeDelta);
            static void SetSimulationStep(float timeDelta);
            static float GetSimulationStep();

		};

	}
}

#endif // !PHYSICSUTILS_H
