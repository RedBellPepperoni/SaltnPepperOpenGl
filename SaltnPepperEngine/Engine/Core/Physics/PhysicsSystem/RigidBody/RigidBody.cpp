#include "RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		void RigidBody::DebugDraw(DebugMode mode)
		{
			//Vector4 color(1.0f, 0.3f, 0.7f, 1.0f);


			//if (!IsStatic())
			//{
			//	// Make it green to show it is doing physics
			//	color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			//}

			//BoundingBox box = GetAABB();
			//



			//switch (mode)
			//{
			//case DebugMode::NONE:
			//	break;
			//case DebugMode::AABB:


			//	DebugRenderer::DebugDraw(box, color, false);

			//	break;

			//case DebugMode::COLLIDER:


			//	


			//	break;
			//case DebugMode::ALL:
			//	break;
			//default:
			//	break;
			//}

			//DebugRenderer::DebugDraw(box, color, true);
		}

		void RigidBody::UpdateTransform(Transform& ecsTransform)
		{
			Vector3 selfScale = ecsTransform.GetScale();



			// If the Body is kinematic, update the Rigidbody's transform usingthe ECS transform
			if (IsKinematic())
			{
				btTransform transform;
				ToBulletTransform(ecsTransform, transform);
				bodyHandle->getMotionState()->setWorldTransform(transform);

			}

			// If its not kinematic, the Transform should be controlled by the Bullet Engine
			else if (HasTransformUpdate())
			{

				btTransform Trans = bodyHandle->getWorldTransform();
				FromBulletTransform(Trans, ecsTransform);
				SetTransformUpdateFlag(false);

			}


			if (selfScale != GetScale())
			{
				SetScale(selfScale);
			}

			DebugDraw(debugMode);

		}

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

		void RigidBody::SetEntityId(Entity parentId)
		{
			entityID = parentId;
		}

		Entity RigidBody::GetEntityId() const
		{
			return entityID;
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

		bool RigidBody::HasTransformUpdate()
		{
			return GetMotionState()->TransformUpdated;
		}

		void RigidBody::SetTransformUpdateFlag(bool value)
		{
			GetMotionState()->TransformUpdated = value;
		}

		RigidBody::RigidBody(const Transform& ecstransform, btCollisionShape* shape)
		{
			// Assign the Shape Reference
			shapeHandle = shape;

			// Convert and create a Transfrom from the worldPosition
			/*btVector3 pos = ToBulletVector3(position);
			btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), pos);*/

			btTransform transform; 
			ToBulletTransform(ecstransform, transform);

			motionHandle = MakeShared<MotionStateNotifier>(transform);

			btRigidBody::btRigidBodyConstructionInfo info(10, motionHandle.get(), shape, btVector3(0, 0, 0));
			//info.m_startWorldTransform = transform;
			//bodyHandle = new btRigidBody(info);
			bodyHandle = MakeShared<btRigidBody>(info);

			SetBounceFactor(0.1f);
			PhysicsUtils::SetRigidBodyParent(bodyHandle.get(),this);
			PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle(), collisionGroup, collisionMask);
			//PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle());

			//bodyHandle->setWorldTransform(transform);

		}

		RigidBody::~RigidBody()
		{
			DestroyBody();
		}

		void RigidBody::ForceTransform(Transform& ecsTransform)
		{
			btTransform transform;
			ToBulletTransform(ecsTransform, transform);
			bodyHandle->setWorldTransform(transform);
		}

		void RigidBody::SetScale(const Vector3& scale)
		{
			if (shapeHandle != nullptr)
			{
				shapeHandle->setLocalScaling(ToBulletVector3(scale));
			}
		}

		Vector3 RigidBody::GetScale()
		{
			if (shapeHandle != nullptr)
			{
				return FromBulletVector3(shapeHandle->getLocalScaling());
			}
			else
			{
				return Vector3{ 0.0f };

			}
		}


		void RigidBody::Activate()
		{
			bodyHandle->activate(true);
		}

		const bool RigidBody::IsActive() const
		{
			return bodyHandle->isActive();
		}

		void RigidBody::SetCollisionShape(btCollisionShape* newShape)
		{
			UpdateCollider(GetMass(), newShape);
		}

		BoundingBox RigidBody::GetAABB()
		{
			BoundingBox result{};

			

			if (shapeHandle != nullptr)
			{
				btVector3 min, max;
				btTransform& tr = bodyHandle->getWorldTransform();
				shapeHandle->getAabb(tr, min, max);
				result.m_min = FromBulletVector3(min);
				result.m_max = FromBulletVector3(max);
			}

			return result;
		}

		void RigidBody::ClearForces()
		{
			bodyHandle->clearForces();
		}

		bool RigidBody::IsMoving() const
		{
			return shapeHandle != nullptr && !shapeHandle->isNonMoving();
		}

		void RigidBody::SetMass(const float value)
		{
			UpdateCollider(value, shapeHandle);
		}

		const float RigidBody::GetMass() const
		{
			return static_cast<float>(bodyHandle->getMass());
		}


		void RigidBody::SetFriction(float value)
		{
			bodyHandle->setFriction(value);
		}

		const float RigidBody::GetFriction() const
		{
			return bodyHandle->getFriction();
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

		
		bool RigidBody::HasCollisionResponse() const
		{
			return !(bodyHandle->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);;
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

		void RigidBody::MakeTrigger()
		{
			SetTriggerFlag();
		}

		const bool RigidBody::IsTrigger() const
		{
			return !HasCollisionResponse();
		}

		const bool RigidBody::IsRayCastable() const
		{
			return GetCollisionGroup() & CollisionGroup::RAYCAST_ONLY;
		}

		void RigidBody::ToggleRayCasting(bool value)
		{
			if (value)
			{
				SetCollisionFilter(this->GetCollisionMask(), this->GetCollisionGroup() | CollisionGroup::RAYCAST_ONLY);
			}
			else
			{
				SetCollisionFilter(this->GetCollisionMask(), this->GetCollisionGroup() & ~CollisionGroup::RAYCAST_ONLY);
			}
		}

		float RigidBody::GetBounceFactor() const
		{
			return bodyHandle->getRestitution();
		}

		void RigidBody::SetBounceFactor(float value)
		{
			bodyHandle->setRestitution(value);
		}


		void RigidBody::ApplyCentralImpulse(Vector3 impulse)
		{
			Activate();
			bodyHandle->applyCentralImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyCentralPushImpulse(Vector3 impulse)
		{
			Activate();
			bodyHandle->applyCentralPushImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyForce(const Vector3& force, const Vector3& relativePosition)
		{
			Activate();
			bodyHandle->applyForce(ToBulletVector3(force), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			Activate();
			bodyHandle->applyImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			Activate();
			bodyHandle->applyPushImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyTorque(const Vector3& force)
		{
			Activate();
			bodyHandle->applyTorque(ToBulletVector3(force));
		}

		void RigidBody::ApplyTorqueImpulse(const Vector3& impulse)
		{
			Activate();
			bodyHandle->applyTorqueImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyTorqueTurnImpulse(const Vector3& impulse)
		{
			Activate();
			bodyHandle->applyTorqueTurnImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyCentralForce(const Vector3& force)
		{
			Activate();
			bodyHandle->applyCentralForce(ToBulletVector3(force));
		}

		void RigidBody::SetLinearVelocity(const Vector3& velocity)
		{
			Activate();
			bodyHandle->setLinearVelocity(ToBulletVector3(velocity));
		}

		const Vector3 RigidBody::GetLinearVelocity() const
		{
			return FromBulletVector3(bodyHandle->getLinearVelocity());
		}
	}
}