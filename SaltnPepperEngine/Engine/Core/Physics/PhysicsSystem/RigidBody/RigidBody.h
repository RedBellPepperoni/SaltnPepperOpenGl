#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include"Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsSystem.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		namespace CollisionMask
		{
			enum Mask : uint32_t
			{
				NONE = 0,
				STATIC = 1,
				KINEMATIC = 1 << 1,
				DYNAMIC = 1 << 2,
				RAYCAST_ONLY = 1 << 3
			};
		}

		namespace CollisionGroup
		{
			enum Group : uint32_t
			{
				NONE = 0,
				RAYCAST_ONLY = CollisionMask::RAYCAST_ONLY,
				DYNAMIC_STATIC_KINEMATTIC = CollisionMask::DYNAMIC | CollisionMask::STATIC | CollisionMask::KINEMATIC,
				ALL_NO_RAYCAST = DYNAMIC_STATIC_KINEMATTIC,
				ALL = DYNAMIC_STATIC_KINEMATTIC | CollisionMask::RAYCAST_ONLY,
				NO_STATIC_COLLISIONS = ALL & ~CollisionMask::STATIC & ~CollisionMask::KINEMATIC,
				NO_STATIC_COLLISION_NO_RAYCAST = NO_STATIC_COLLISIONS & ~CollisionMask::RAYCAST_ONLY
			};
		}



		class MotionStateNotifier : public btDefaultMotionState
		{
		public:
			MotionStateNotifier(btTransform& tr) : btDefaultMotionState(tr) { }

			bool TransformUpdated = true;

			virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
			{
				this->TransformUpdated = true;
				btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
			}
		};

		class RigidBody
		{
		private:

			SharedPtr<btRigidBody> bodyHandle = nullptr;
			//btMotionState* motionHandle = nullptr;
			SharedPtr<MotionStateNotifier> motionHandle = nullptr;

			btCollisionShape* shapeHandle = nullptr;

			uint32_t collisionMask = CollisionGroup::NO_STATIC_COLLISIONS;;
			uint32_t collisionGroup = CollisionMask::STATIC;

		private:

			void DestroyBody();

			void ReAddRigidBody();

			void UpdateCollider(float mass, btCollisionShape* newShape);

		public:

			RigidBody(const Vector3& position, btCollisionShape* shape);
			
			~RigidBody();
		

			btRigidBody* GetNativeHandle() { return bodyHandle.get(); }
			const btRigidBody* GetNativeHandle() const { return bodyHandle.get(); }

			MotionStateNotifier* GetMotionState() { return motionHandle.get(); }
			const MotionStateNotifier* GetMotionState() const { return motionHandle.get(); }

			btCollisionShape* GetCollisionShape() { return shapeHandle; }
			const btCollisionShape* GetCollisionShape() const{ return shapeHandle; }

			void SetMass(const float value);
			const float GetMass() const;


		};

	}
}

#endif // !RIGIDBODY_H
