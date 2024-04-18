#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Engine/Core/EntitySystem/EntityManager.h"
#include"Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsSystem.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"


namespace SaltnPepperEngine
{
	//class Entity;

	namespace Physics
	{
		/*namespace CollisionMask
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
		}*/
		namespace CollisionMask
		{
			enum Mask 
			{
				DYNAMIC = 1,
				STATIC = 2,
				KINEMATIC = 4,
				RAYCAST = 8,
				TRIGGER = 16,
				CHARACTER = 32,
				AllFilter = -1  //all bits sets: DefaultFilter | StaticFilter | KinematicFilter | DebrisFilter | SensorTrigger
			};
		}

		namespace CollisionGroup
		{
			enum Group 
			{
				NONE = 0,
				RAYCAST_ONLY = CollisionMask::RAYCAST,
				DYNAMIC_STATIC_KINEMATTIC = CollisionMask::DYNAMIC | CollisionMask::STATIC | CollisionMask::KINEMATIC,
				ALL = DYNAMIC_STATIC_KINEMATTIC | CollisionMask::RAYCAST,
				NO_STATIC_COLLISIONS = ALL & ~CollisionMask::STATIC & ~CollisionMask::KINEMATIC,
				NO_STATIC_COLLISION_NO_RAYCAST = NO_STATIC_COLLISIONS & ~CollisionMask::RAYCAST

				
			};
		}



		enum class ActivationState
		{
			ACTIVETAG = 1,
			ISLANDSLEEPING = 2,
			WANTSDEACTIVATION = 3,
			DISABLEDEACTIVATION = 4,
			DISABLESIMULATION = 5
		};


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
			using CollisionCallback = std::function<void(RigidBody*, RigidBody*)>;

		public:

			enum class DebugMode : uint8_t
			{
				NONE = 0,
				AABB = 1,
				COLLIDER = 2,
				ALL = 3
			};

		private:



			SharedPtr<btRigidBody> bodyHandle = nullptr;
			//btMotionState* motionHandle = nullptr;
			SharedPtr<MotionStateNotifier> motionHandle = nullptr;

			btCollisionShape* shapeHandle = nullptr;

			uint32_t collisionMask = CollisionMask::DYNAMIC;
			uint32_t collisionGroup = CollisionGroup::ALL;

			CollisionCallback onCollision;
			CollisionCallback onCollisionEnter;
			CollisionCallback onCollisionExit;

			DebugMode debugMode = DebugMode::AABB;

			Entity entityID;


		private:

			void DestroyBody();

			void ReAddRigidBody();

			void UpdateCollider(float mass, btCollisionShape* newShape);

		

			// Collision Flags : KINEMATIC
			void SetKinematicFlag();
			void UnsetKinematicFlag();

			// Collision Flags : TRIGEGR
			void SetTriggerFlag();
			void UnsetTriggerFlag();

			void UnsetAllFlags();

			bool HasTransformUpdate();

			void SetTransformUpdateFlag(bool value);
			
			void DebugDraw(DebugMode mode);
		public:

			

			RigidBody(const Transform& ecstransform, btCollisionShape* shape);
			
			~RigidBody();


			void ForceTransform(Transform& ecsTransform);

		
			void SetEntityId(Entity parentId);
			Entity GetEntityId() const;

			void UpdateTransform(Transform& ecsTransform);

			Vector3 GetScale();
			void SetScale(const Vector3& scale);

			void Activate();
			const bool IsActive() const;

			btRigidBody* GetNativeHandle() { return bodyHandle.get(); }
			const btRigidBody* GetNativeHandle() const { return bodyHandle.get(); }

			MotionStateNotifier* GetMotionState() { return motionHandle.get(); }
			const MotionStateNotifier* GetMotionState() const { return motionHandle.get(); }

			btCollisionShape* GetCollisionShape() { return shapeHandle; }
			const btCollisionShape* GetCollisionShape() const{ return shapeHandle; }

			void SetCollisionShape(btCollisionShape* newShape);

			BoundingBox GetAABB();

			void ClearForces();

			void SetMass(const float value);
			const float GetMass() const;

			
			void SetFriction(float value);
			const float GetFriction() const;

			void SetCollisionFilter(uint32_t mask, uint32_t group = CollisionGroup::Group::ALL);
			void SetCollisionFilter(CollisionMask::Mask mask, CollisionGroup::Group group = CollisionGroup::Group::ALL);

			const uint32_t GetCollisionGroup() const { return collisionGroup; }
			const uint32_t GetCollisionMask() const { return collisionMask; }

			void SetActivationState(ActivationState state);
			ActivationState GetActivationState() const;

			bool HasCollisionResponse() const;
			bool IsMoving() const;

			void MakeKinematic();
			const bool IsKinematic() const;

			void MakeDynamic();
			const bool IsDynamic() const;

			void MakeStatic();
			const bool IsStatic() const;

			void MakeTrigger();
			const bool IsTrigger() const;
			
			const bool IsRayCastable() const;
			void ToggleRayCasting(bool value);


			float GetBounceFactor() const;
			void SetBounceFactor(float value);

			void ApplyCentralImpulse(Vector3 impulse);
			void ApplyCentralPushImpulse(Vector3 impulse);

			void ApplyForce(const Vector3& force, const Vector3& relativePosition);
			void ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition);

			void ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition);
			void ApplyTorque(const Vector3& force);

			void ApplyTorqueImpulse(const Vector3& impulse);
			void ApplyTorqueTurnImpulse(const Vector3& impulse);

			void ApplyCentralForce(const Vector3& force);

			void SetLinearVelocity(const Vector3& velocity);
			const Vector3 GetLinearVelocity() const;


		};

	}
}

#endif // !RIGIDBODY_H
