#include "RigidBody_Dep.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/RigidBody/BulletRigidBody.h"

#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"

#include "Engine/Core/Physics/PhysicsSystem/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CapsuleCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CylinderCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

		void RigidBody_Dep::UpdateTransform(Transform& ECSTransform) 
		{ 
			Vector3 selfScale = ECSTransform.GetScale();

			

			// If the Body is kinematic, update the Rigidbody's transform usingthe ECS transform
			if (IsKinematic())
			{
				localTransform = ECSTransform;
				btTransform transform;
				ToBulletTransform(ECSTransform, transform);
				rigidbody->GetNativeHandle()->getMotionState()->setWorldTransform(transform);

			}

			// If its not kinematic, the Transform should be controlled by the Bullet Engine
			else if (rigidbody->HasTransformUpdate())
			{
				
				btTransform Trans = rigidbody->GetNativeHandle()->getWorldTransform();
				FromBulletTransform(Trans, localTransform);
				rigidbody->SetTransformUpdateFlag(false);

				ECSTransform = localTransform;

			}


			if (selfScale != rigidbody->GetScale())
			{
				rigidbody->SetScale(selfScale);
				localTransform.SetScale(selfScale);
			}

		}


		template< typename T>
		bool TestCollider(BulletRigidBody* rigidBody, T* collider)
		{
			if (collider == nullptr) {return false;}
			if (rigidBody == nullptr) { return false;}

			if (collider->HasColliderChanged())
			{
				btCollisionShape* shape = collider->GetShape();
				rigidBody->SetCollisionShape(shape);
				collider->SetColliderChangedFlag(false);
			}

			return true;

		}

		template<typename T>
		void InvalidateCollider(T* collider)
		{
			if (collider != nullptr)
			{
				collider->SetColliderChangedFlag(true);
			}
		}

		void RigidBody_Dep::UpdateCollider(BaseCollider* Collider)
		{
			if (Collider == nullptr)
			{
				rigidbody->SetCollisionShape(nullptr);
			}

			if (TestCollider(rigidbody.get(), reinterpret_cast<BoxCollider*>(Collider))) return ;
			if (TestCollider(rigidbody.get(), reinterpret_cast<SphereCollider*>(Collider))) return;
			if (TestCollider(rigidbody.get(), reinterpret_cast<CapsuleCollider*>(Collider))) return;
			if (TestCollider(rigidbody.get(), reinterpret_cast<CylinderCollider*>(Collider))) return;
		}



		void RigidBody_Dep::Init(const Transform& transform, BaseCollider* collider)
		{

			rigidbody = MakeShared<BulletRigidBody>(transform);
		
			//PhysicsUtils::SetRigidBodyParent(rigidbody->GetNativeHandle(), this);
		
			if (collider == nullptr) { return; }

			if (TestCollider(rigidbody.get(), reinterpret_cast<BoxCollider*>(collider))) return;
			if (TestCollider(rigidbody.get(), reinterpret_cast<SphereCollider*>(collider))) return;
			if (TestCollider(rigidbody.get(), reinterpret_cast<CapsuleCollider*>(collider))) return;
			if (TestCollider(rigidbody.get(), reinterpret_cast<CylinderCollider*>(collider))) return;

			/*InvalidateCollider(reinterpret_cast<BoxCollider*>(collider));
			InvalidateCollider(reinterpret_cast<SphereCollider*>(collider));
			InvalidateCollider(reinterpret_cast<CapsuleCollider*>(collider));
			InvalidateCollider(reinterpret_cast<CylinderCollider*>(collider));*/

		}

		void RigidBody_Dep::OnUpdate(float dt, Transform& transform)
		{
			UpdateTransform(transform);
		}

	

		BulletRigidBody* RigidBody_Dep::GetBulletHandle() const
		{
			return rigidbody.get();
		}

		void RigidBody_Dep::InvokeOnCollisionCallback(RigidBody_Dep* self, RigidBody_Dep* object)
		{
			if (onCollision)
			{
				onCollision(std::move(self), std::move(object));
			}
		}

		void RigidBody_Dep::InvokeOnCollisionEnterCallback(RigidBody_Dep* self, RigidBody_Dep* object)
		{
			if (onCollisionEnter)
			{
				onCollisionEnter(std::move(self), std::move(object));
			}
		}

		void RigidBody_Dep::InvokeOnCollisionExitCallback(RigidBody_Dep* self, RigidBody_Dep* object)
		{
			if (onCollisionExit)
			{
				onCollisionExit(std::move(self), std::move(object));
			}
		}

		void RigidBody_Dep::MakeKinematic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionMask::KINEMATIC, CollisionGroup::NO_STATIC_COLLISIONS);
			rigidbody->SetKinematicFlag();
		}

		void RigidBody_Dep::MakeDynamic()
		{
			if (GetMass() == 0.0f) { SetMass(1.0f); }

			SetCollisionFilter(CollisionMask::DYNAMIC, CollisionGroup::ALL);

			rigidbody->UnsetKinematicFlag();
		}

		void RigidBody_Dep::MakeStatic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionMask::STATIC, CollisionGroup::NO_STATIC_COLLISIONS);
			rigidbody->UnsetKinematicFlag();

		}

		void RigidBody_Dep::MakeTrigger()
		{
			rigidbody->SetTriggerFlag();
		}

		bool RigidBody_Dep::IsKinematic() const
		{
			return GetCollisionLayer() & CollisionMask::KINEMATIC;
		}

		bool RigidBody_Dep::IsDynamic() const
		{
			return GetCollisionLayer() & CollisionMask::DYNAMIC;
		}

		bool RigidBody_Dep::IsStatic() const
		{
			return GetCollisionLayer() & CollisionMask::STATIC;
		}

		bool RigidBody_Dep::IsTrigger() const
		{
			return !rigidbody->HasCollisionResponce();
		}

		bool RigidBody_Dep::IsRayCastable() const
		{
			return this->GetCollisionGroup() & CollisionGroup::RAYCAST_ONLY;
		}

		void RigidBody_Dep::ToggleRayCasting(bool value)
		{
			if (value)
			{
				this->SetCollisionFilter(this->GetCollisionLayer(), this->GetCollisionGroup() | CollisionGroup::RAYCAST_ONLY);
			}
			else
			{
				this->SetCollisionFilter(this->GetCollisionLayer(), this->GetCollisionGroup() & ~CollisionGroup::RAYCAST_ONLY);
			}
		}

		bool RigidBody_Dep::IsMoving() const
		{
			return rigidbody->IsMoving();
		}

		void RigidBody_Dep::ToggleTrigger(bool value)
		{
			if (value) { MakeTrigger(); }
		}

		RigidBodyType RigidBody_Dep::GetTypeInternal() const
		{
			return  this->IsStatic() ? RigidBodyType::STATIC :
					this->IsDynamic() ? RigidBodyType::DYNAMIC :
					this->IsKinematic() ? RigidBodyType::KINEMATIC :
					RigidBodyType::STATIC;
		}

		void RigidBody_Dep::SetTypeInternal(RigidBodyType type)
		{
			switch (type)
			{
			case RigidBodyType::STATIC:
				this->MakeStatic();
				break;
			case RigidBodyType::DYNAMIC:
				this->MakeDynamic();
				break;
			case RigidBodyType::KINEMATIC:
				this->MakeKinematic();
				break;
			default: // do not do anything
				break;
			}
		}

		void RigidBody_Dep::SetCollisionFilter(uint32_t mask, uint32_t group)
		{
			rigidbody->SetCollisionFilter(group, mask);
		}

		void RigidBody_Dep::SetCollisionFilter(CollisionMask::Mask mask, CollisionGroup::Group group)
		{
			SetCollisionFilter((uint32_t)mask, (uint32_t)group);
		}

		uint32_t RigidBody_Dep::GetCollisionGroup() const
		{
			return rigidbody->GetCollisionGroup();
		}

		uint32_t RigidBody_Dep::GetCollisionLayer() const
		{
			return rigidbody->GetCollisionMask();
		}

		void RigidBody_Dep::ActivateParentIsland()
		{
			PhysicsUtils::ActiveRigidBodyIsland(rigidbody->GetNativeHandle());
		}

		void RigidBody_Dep::SetActivationState(ActivationState state)
		{
			rigidbody->SetActivationState(state);
		}

		ActivationState RigidBody_Dep::GetActivationState() const
		{
			return rigidbody->GetActivationState();
		}

		BoundingBox RigidBody_Dep::GetAABB() const
		{
			BoundingBox result{};

			btCollisionShape* collider = rigidbody->GetCollisionShape();

			if (collider != nullptr)
			{
				btVector3 min, max;
				btTransform& tr = rigidbody->GetNativeHandle()->getWorldTransform();
				collider->getAabb(tr, min, max);
				result.m_min = FromBulletVector3(min);
				result.m_max = FromBulletVector3(max);
			}

			return result;
		}

		void RigidBody_Dep::Activate()
		{
			rigidbody->Activate();
		}

		void RigidBody_Dep::ClearForces()
		{
			rigidbody->GetNativeHandle()->clearForces();
		}

		float RigidBody_Dep::GetMass() const
		{
			return rigidbody->GetMass();
		}

		void RigidBody_Dep::SetMass(float mass)
		{
			rigidbody->SetMass(mass);
		}

		float RigidBody_Dep::GetFriction() const
		{
			return rigidbody->GetNativeHandle()->getFriction();
		}

		void RigidBody_Dep::SetFriction(float value)
		{
			rigidbody->GetNativeHandle()->setFriction(value);
		}

		float RigidBody_Dep::GetSpinningFriction() const
		{
			return rigidbody->GetNativeHandle()->getSpinningFriction();
		}

		void RigidBody_Dep::SetSpinningFriction(float value)
		{
			rigidbody->GetNativeHandle()->setSpinningFriction(value);
		}

		float RigidBody_Dep::GetRollingFriction() const
		{
			return rigidbody->GetNativeHandle()->getRollingFriction();
		}

		void RigidBody_Dep::SetRollingFriction(float value)
		{
			rigidbody->GetNativeHandle()->setRollingFriction(value);
		}

		float RigidBody_Dep::GetBounceFactor() const
		{
			return rigidbody->GetNativeHandle()->getRestitution();
		}
		void RigidBody_Dep::SetBounceFactor(float value)
		{
			rigidbody->GetNativeHandle()->setRestitution(value);
		}

		void RigidBody_Dep::SetGravity(Vector3 gravity)
		{
			rigidbody->GetNativeHandle()->setGravity(ToBulletVector3(gravity));
		}

		Vector3 RigidBody_Dep::GetGravity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getGravity());
		}

		void RigidBody_Dep::SetLinearVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setLinearVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody_Dep::GetLinearVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getLinearVelocity());
		}

		void RigidBody_Dep::SetAngularVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setAngularVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody_Dep::GetAngularVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAngularVelocity());
		}

		float RigidBody_Dep::GetInverseMass() const
		{
			return rigidbody->GetNativeHandle()->getInvMass();
		}

		Vector3 RigidBody_Dep::GetAngularForceFactor() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAngularFactor());
		}

		void RigidBody_Dep::SetAngularForceFactor(Vector3 factor)
		{
			rigidbody->GetNativeHandle()->setAngularFactor(ToBulletVector3(factor));
		}

		Vector3 RigidBody_Dep::GetLinearForceFactor() const
		{
			return FromBulletVector3(this->rigidbody->GetNativeHandle()->getLinearFactor());
		}

		void RigidBody_Dep::SetLinearForceFactor(Vector3 factor)
		{
			rigidbody->GetNativeHandle()->setLinearFactor(ToBulletVector3(factor));
		}

		void RigidBody_Dep::SetAngularAirResistance(float value)
		{
			rigidbody->GetNativeHandle()->setDamping(this->GetLinearAirResistance(), value);
		}

		float RigidBody_Dep::GetAngularAirResistance() const
		{
			return rigidbody->GetNativeHandle()->getAngularDamping();;
		}

		void RigidBody_Dep::SetLinearAirResistance(float value)
		{
			rigidbody->GetNativeHandle()->setDamping(value, this->GetAngularAirResistance());
		}

		float RigidBody_Dep::GetLinearAirResistance() const
		{
			return rigidbody->GetNativeHandle()->getLinearDamping();
		}

		Vector3 RigidBody_Dep::GetTotalForceApplied() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTotalForce());
		}

		Vector3 RigidBody_Dep::GetTotalTorqueApplied() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTotalTorque());
		}

		Vector3 RigidBody_Dep::GetInertia() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getLocalInertia());
		}

		Vector3 RigidBody_Dep::GetVelocityInPoint(Vector3 relativePosition)
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getVelocityInLocalPoint(ToBulletVector3(relativePosition)));
		}

		Vector3 RigidBody_Dep::GetPushVelocityInPoint(Vector3 relativePosition)
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getPushVelocityInLocalPoint(ToBulletVector3(relativePosition)));
		}

		void RigidBody_Dep::ApplyCentralImpulse(Vector3 impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralImpulse(ToBulletVector3(impulse));
		}

		void RigidBody_Dep::ApplyCentralPushImpulse(Vector3 impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralPushImpulse(ToBulletVector3(impulse));
		}

		void RigidBody_Dep::ApplyForce(const Vector3& force, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyForce(ToBulletVector3(force), ToBulletVector3(relativePosition));
		}

		void RigidBody_Dep::ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody_Dep::ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyPushImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody_Dep::ApplyTorque(const Vector3& force)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorque(ToBulletVector3(force));
		}

		void RigidBody_Dep::ApplyTorqueImpulse(const Vector3& impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorqueImpulse(ToBulletVector3(impulse));
		}

		void RigidBody_Dep::ApplyTorqueTurnImpulse(const Vector3& impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorqueTurnImpulse(ToBulletVector3(impulse));
		}

		void RigidBody_Dep::ApplyCentralForce(const Vector3& force)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralForce(ToBulletVector3(force));
		}

		void RigidBody_Dep::SetPushVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setPushVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody_Dep::GetPushVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getPushVelocity());
		}

		void RigidBody_Dep::SetTurnVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setTurnVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody_Dep::GetTurnVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTurnVelocity());
		}

		void RigidBody_Dep::SetAnisotropicFriction(Vector3 friction, AnisotropicFriction mode)
		{
			rigidbody->GetNativeHandle()->setAnisotropicFriction(ToBulletVector3(friction), (int)mode);
		}

		void RigidBody_Dep::SetAnisotropicFriction(Vector3 friction)
		{
			SetAnisotropicFriction(friction, HasAnisotropicFriction() ? AnisotropicFriction::ENABLED : AnisotropicFriction::DISABLED);
		}

		bool RigidBody_Dep::HasAnisotropicFriction() const
		{
			return rigidbody->GetNativeHandle()->hasAnisotropicFriction();
		}

		void RigidBody_Dep::ToggleAnisotropicFriction(bool value)
		{
			SetAnisotropicFriction(GetAnisotropicFriction(), value ? AnisotropicFriction::ENABLED : AnisotropicFriction::DISABLED);
		}

		Vector3 RigidBody_Dep::GetAnisotropicFriction() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAnisotropicFriction());
		}
	}
}