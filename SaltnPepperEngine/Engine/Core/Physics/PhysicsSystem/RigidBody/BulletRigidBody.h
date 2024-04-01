#ifndef BULLETRIGIDBODY_H
#define BULLETRIGIDBODY_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/Transform.h"
#include "RigidBody.h"

class btRigidBody;
class btCollisionShape;
class btMotionState;


namespace SaltnPepperEngine
{
	using Components::Transform;

	namespace Physics
	{
		//class MotionStateNotifier;


		/*namespace CollisionLayer
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
				RAYCAST_ONLY = CollisionLayer::RAYCAST_ONLY,
				DYNAMIC_STATIC_KINEMATTIC = CollisionLayer::DYNAMIC | CollisionLayer::STATIC | CollisionLayer::KINEMATIC,
				ALL_NO_RAYCAST = DYNAMIC_STATIC_KINEMATTIC,
				ALL = DYNAMIC_STATIC_KINEMATTIC | CollisionLayer::RAYCAST_ONLY,
				NO_STATIC_COLLISIONS = ALL & ~CollisionLayer::STATIC & ~CollisionLayer::KINEMATIC,
				NO_STATIC_COLLISION_NO_RAYCAST = NO_STATIC_COLLISIONS & ~CollisionLayer::RAYCAST_ONLY
			};
		}
		*/
	/*	enum class ActivationState
		{
			ACTIVE_TAG = 1,
			ISLAND_SLEEPING = 2,
			WANTS_DEACTIVATION = 3,
			DISABLE_DEACTIVATION = 4,
			DISABLE_SIMULATION = 5
		}*/

		//enum class ActivationState
		//{
		//	ACTIVETAG = 1,
		//	ISLANDSLEEPING = 2,
		//	WANTSDEACTIVATION = 3,
		//	DISABLEDEACTIVATION = 4,
		//	DISABLESIMULATION = 5
		//};

		class BulletRigidBody
		{
			//struct RigidBodyAllocation;
			//SharedPtr<RigidBodyAllocation> m_bodyAllocation = nullptr;

			btRigidBody* Body = nullptr;
			MotionStateNotifier* MotionState = nullptr;
		
			uint32_t m_group = CollisionGroup::NO_STATIC_COLLISIONS;
			uint32_t m_layer = CollisionMask::STATIC;

			void DestroyBody();
			void ReAddRigidBody();
			void UpdateCollider(float mass, btCollisionShape* collider);
		
		public:
			
			BulletRigidBody(const Transform& transform);
			BulletRigidBody(const Transform& transform, btCollisionShape* collider);
			BulletRigidBody(const BulletRigidBody&) = delete;
			BulletRigidBody(BulletRigidBody&& other) noexcept;
			BulletRigidBody& operator=(const BulletRigidBody&) = delete;
			BulletRigidBody& operator=(BulletRigidBody&& other ) noexcept;
			~BulletRigidBody();


			btRigidBody* GetNativeHandle(); 
			const btRigidBody* GetNativeHandle() const;
			btMotionState* GetMotionState();
			const btMotionState* GetMotionState() const;
			bool HasTransformUpdate() const;
			void SetTransformUpdateFlag(bool value);

			btCollisionShape* GetCollisionShape();
			const btCollisionShape* GetCollisionShape() const;
			void SetCollisionShape(btCollisionShape* shape);
			void SetCollisionFilter(uint32_t group, uint32_t layer);
			uint32_t GetCollisionGroup() const;
			uint32_t GetCollisionMask() const;
			Vector3 GetScale() const;
			bool IsMoving() const;
			bool HasCollisionResponce() const;
			void SetKinematicFlag();
			void UnsetKinematicFlag();
			void SetTriggerFlag();
			void UnsetTriggerFlag();
			void UnsetAllFlags();
			void SetScale(const Vector3& scale);
			float GetMass() const;
			void SetMass(float mass);
			void SetActivationState(ActivationState state);
			ActivationState GetActivationState() const;
			void Activate();
			bool IsActive() const;
		};

	}
}

#endif // !BULLETRIGIDBODY_H

