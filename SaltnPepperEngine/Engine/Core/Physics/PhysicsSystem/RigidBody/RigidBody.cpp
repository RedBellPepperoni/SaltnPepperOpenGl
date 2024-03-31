#include "RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		void RigidBody::DestroyBody()
		{
			if (bodyHandle)
			{
				PhysicsUtils::RemoveRigidBody(GetNativeHandle());

				motionHandle->~MotionStateNotifier();
				bodyHandle->~btRigidBody();
			}

		}

		///  Do not Use this in Update() performance intensive
		void RigidBody::ReAddRigidBody()
		{
			// get a temporary Reference to the currently loaded bullet Body
			btRigidBody* tempReference = GetNativeHandle();

			// Remove the Body
			PhysicsUtils::RemoveRigidBody(tempReference);

			// Add The Body (Sorta like a Refesh)
			PhysicsUtils::AddRigidBody(tempReference,collisionGroup,collisionMask);


			
		}

		void RigidBody::UpdateCollider(float mass, btCollisionShape* newShape)
		{

			btVector3 inertia(0.0f, 0.0f, 0.0f);

			// Calculate Intertia if the body is dynamic
			if (shapeHandle != nullptr && mass != 0.0f)
			{
				shapeHandle->calculateLocalInertia(mass, inertia);
			}

			// Set the new mass
			this->GetNativeHandle()->setMassProps(mass, inertia);

			// Update Collider incase needed
			if (shapeHandle != this->GetCollisionShape())
			{
				this->GetNativeHandle()->setCollisionShape(shapeHandle);
				this->ReAddRigidBody();
			}
		}

		void RigidBody::SetKinematicFlag()
		{
			bodyHandle->setCollisionFlags(bodyHandle->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			SetActivationState(ActivationState::DISABLEDEACTIVATION);
		}

		void RigidBody::UnsetKinematicFlag()
		{
			bodyHandle->setCollisionFlags(bodyHandle->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
			SetActivationState(ActivationState::ACTIVETAG);
		}

		void RigidBody::SetTriggerFlag()
		{
			bodyHandle->setCollisionFlags(bodyHandle->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::UnsetTriggerFlag()
		{
			bodyHandle->setCollisionFlags(bodyHandle->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		void RigidBody::UnsetAllFlags()
		{
			UnsetTriggerFlag();
			UnsetKinematicFlag();
		}

		RigidBody::RigidBody(const Vector3& position, btCollisionShape* shape)
		{
			// Assign the Shape Reference
			shapeHandle = shape;

			// Convert and create a Transfrom from the worldPosition
			btVector3 pos = ToBulletVector3(position);
			btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
			motionHandle = MakeShared<MotionStateNotifier>(transform);

			btRigidBody::btRigidBodyConstructionInfo info(10, motionHandle.get(), shape, btVector3(0, 0, 0));
			//bodyHandle = new btRigidBody(info);
			bodyHandle = MakeShared<btRigidBody>(info);

			SetBounceFactor(0.1f);

			PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle(), collisionGroup, collisionMask);


		}

		RigidBody::~RigidBody()
		{
			DestroyBody();
		}


		void RigidBody::Activate()
		{
			bodyHandle->activate(true);
		}

		const bool RigidBody::IsActive() const
		{
			bodyHandle->isActive();
		}

		void RigidBody::SetCollisionShape(btCollisionShape* newShape)
		{
			UpdateCollider(GetMass(), newShape);
		}

		void RigidBody::SetMass(const float value)
		{
			UpdateCollider(value, shapeHandle);
		}

		const float RigidBody::GetMass() const
		{
			return static_cast<float>(bodyHandle->getMass());
		}

		void RigidBody::SetCollisionFilter(uint32_t mask, uint32_t group)
		{
			collisionMask = mask;
			collisionGroup = group;
			ReAddRigidBody();
		}

		void RigidBody::SetCollisionFilter(CollisionMask::Mask mask, CollisionGroup::Group group)
		{
			SetCollisionFilter((uint32_t)mask, (uint32_t)group);
		}
		
		void RigidBody::SetActivationState(ActivationState state)
		{
			bodyHandle->forceActivationState((int)state);
		}

		void RigidBody::MakeKinematic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionMask::KINEMATIC, CollisionGroup::NO_STATIC_COLLISIONS); 
			SetKinematicFlag();
		}

		const bool RigidBody::IsKinematic() const
		{
			return GetCollisionMask() & CollisionMask::KINEMATIC;
		}

		void RigidBody::MakeDynamic()
		{
			if (GetMass() == 0.0f) { SetMass(1.0f); } 
			SetCollisionFilter(CollisionMask::DYNAMIC, CollisionGroup::ALL); 
			UnsetKinematicFlag(); 
		}

		const bool RigidBody::IsDynamic() const
		{
			return GetCollisionMask() & CollisionMask::DYNAMIC;
		}

		void RigidBody::MakeStatic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionMask::STATIC, CollisionGroup::NO_STATIC_COLLISIONS);
			UnsetKinematicFlag();
		}

		const bool RigidBody::IsStatic() const
		{
			return GetCollisionMask() & CollisionMask::STATIC;
		}

		float RigidBody::GetBounceFactor() const
		{
			bodyHandle->getRestitution();
		}

		void RigidBody::SetBounceFactor(float value)
		{
			bodyHandle->setRestitution(value);
		}
	}
}