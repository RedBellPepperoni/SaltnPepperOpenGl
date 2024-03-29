#include "RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"

#include "Engine/Core/Physics/PhysicsSystem/Colliders/BoxCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/SphereCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CapsuleCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/Colliders/CylinderCollider.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{

		void RigidBody::UpdateTransform(Transform& ECSTransform) 
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

		void RigidBody::UpdateCollider(BaseCollider* Collider)
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



		void RigidBody::Init(const Transform& transform, BaseCollider* collider)
		{

			rigidbody = MakeShared<BulletRigidBody>(transform);
		
			PhysicsUtils::SetRigidBodyParent(rigidbody->GetNativeHandle(), this);
		
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

		void RigidBody::OnUpdate(float dt, Transform& transform)
		{
			UpdateTransform(transform);
		}

	

		BulletRigidBody* RigidBody::GetBulletHandle() const
		{
			return rigidbody.get();
		}

		void RigidBody::InvokeOnCollisionCallback(RigidBody* self, RigidBody* object)
		{
			if (onCollision)
			{
				onCollision(std::move(self), std::move(object));
			}
		}

		void RigidBody::InvokeOnCollisionEnterCallback(RigidBody* self, RigidBody* object)
		{
			if (onCollisionEnter)
			{
				onCollisionEnter(std::move(self), std::move(object));
			}
		}

		void RigidBody::InvokeOnCollisionExitCallback(RigidBody* self, RigidBody* object)
		{
			if (onCollisionExit)
			{
				onCollisionExit(std::move(self), std::move(object));
			}
		}

		void RigidBody::MakeKinematic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionLayer::KINEMATIC, CollisionGroup::NO_STATIC_COLLISIONS);
			rigidbody->SetKinematicFlag();
		}

		void RigidBody::MakeDynamic()
		{
			if (GetMass() == 0.0f) { SetMass(1.0f); }

			SetCollisionFilter(CollisionLayer::DYNAMIC, CollisionGroup::ALL);

			rigidbody->UnsetKinematicFlag();
		}

		void RigidBody::MakeStatic()
		{
			SetMass(0.0f);
			SetCollisionFilter(CollisionLayer::STATIC, CollisionGroup::NO_STATIC_COLLISIONS);
			rigidbody->UnsetKinematicFlag();

		}

		void RigidBody::MakeTrigger()
		{
			rigidbody->SetTriggerFlag();
		}

		bool RigidBody::IsKinematic() const
		{
			return GetCollisionLayer() & CollisionLayer::KINEMATIC;
		}

		bool RigidBody::IsDynamic() const
		{
			return GetCollisionLayer() & CollisionLayer::DYNAMIC;
		}

		bool RigidBody::IsStatic() const
		{
			return GetCollisionLayer() & CollisionLayer::STATIC;
		}

		bool RigidBody::IsTrigger() const
		{
			return !rigidbody->HasCollisionResponce();
		}

		bool RigidBody::IsRayCastable() const
		{
			return this->GetCollisionGroup() & CollisionGroup::RAYCAST_ONLY;
		}

		void RigidBody::ToggleRayCasting(bool value)
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

		bool RigidBody::IsMoving() const
		{
			return rigidbody->IsMoving();
		}

		void RigidBody::ToggleTrigger(bool value)
		{
			if (value) { MakeTrigger(); }
		}

		RigidBodyType RigidBody::GetTypeInternal() const
		{
			return  this->IsStatic() ? RigidBodyType::STATIC :
					this->IsDynamic() ? RigidBodyType::DYNAMIC :
					this->IsKinematic() ? RigidBodyType::KINEMATIC :
					RigidBodyType::STATIC;
		}

		void RigidBody::SetTypeInternal(RigidBodyType type)
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

		void RigidBody::SetCollisionFilter(uint32_t mask, uint32_t group)
		{
			rigidbody->SetCollisionFilter(group, mask);
		}

		void RigidBody::SetCollisionFilter(CollisionLayer::Mask mask, CollisionGroup::Group group)
		{
			SetCollisionFilter((uint32_t)mask, (uint32_t)group);
		}

		uint32_t RigidBody::GetCollisionGroup() const
		{
			return rigidbody->GetCollisionGroup();
		}

		uint32_t RigidBody::GetCollisionLayer() const
		{
			return rigidbody->GetCollisionMask();
		}

		void RigidBody::ActivateParentIsland()
		{
			PhysicsUtils::ActiveRigidBodyIsland(rigidbody->GetNativeHandle());
		}

		void RigidBody::SetActivationState(ActivationState state)
		{
			rigidbody->SetActivationState(state);
		}

		ActivationState RigidBody::GetActivationState() const
		{
			return rigidbody->GetActivationState();
		}

		BoundingBox RigidBody::GetAABB() const
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

		void RigidBody::Activate()
		{
			rigidbody->Activate();
		}

		void RigidBody::ClearForces()
		{
			rigidbody->GetNativeHandle()->clearForces();
		}

		float RigidBody::GetMass() const
		{
			return rigidbody->GetMass();
		}

		void RigidBody::SetMass(float mass)
		{
			rigidbody->SetMass(mass);
		}

		float RigidBody::GetFriction() const
		{
			return rigidbody->GetNativeHandle()->getFriction();
		}

		void RigidBody::SetFriction(float value)
		{
			rigidbody->GetNativeHandle()->setFriction(value);
		}

		float RigidBody::GetSpinningFriction() const
		{
			return rigidbody->GetNativeHandle()->getSpinningFriction();
		}

		void RigidBody::SetSpinningFriction(float value)
		{
			rigidbody->GetNativeHandle()->setSpinningFriction(value);
		}

		float RigidBody::GetRollingFriction() const
		{
			return rigidbody->GetNativeHandle()->getRollingFriction();
		}

		void RigidBody::SetRollingFriction(float value)
		{
			rigidbody->GetNativeHandle()->setRollingFriction(value);
		}

		float RigidBody::GetBounceFactor() const
		{
			return rigidbody->GetNativeHandle()->getRestitution();
		}
		void RigidBody::SetBounceFactor(float value)
		{
			rigidbody->GetNativeHandle()->setRestitution(value);
		}

		void RigidBody::SetGravity(Vector3 gravity)
		{
			rigidbody->GetNativeHandle()->setGravity(ToBulletVector3(gravity));
		}

		Vector3 RigidBody::GetGravity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getGravity());
		}

		void RigidBody::SetLinearVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setLinearVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody::GetLinearVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getLinearVelocity());
		}

		void RigidBody::SetAngularVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setAngularVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody::GetAngularVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAngularVelocity());
		}

		float RigidBody::GetInverseMass() const
		{
			return rigidbody->GetNativeHandle()->getInvMass();
		}

		Vector3 RigidBody::GetAngularForceFactor() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAngularFactor());
		}

		void RigidBody::SetAngularForceFactor(Vector3 factor)
		{
			rigidbody->GetNativeHandle()->setAngularFactor(ToBulletVector3(factor));
		}

		Vector3 RigidBody::GetLinearForceFactor() const
		{
			return FromBulletVector3(this->rigidbody->GetNativeHandle()->getLinearFactor());
		}

		void RigidBody::SetLinearForceFactor(Vector3 factor)
		{
			rigidbody->GetNativeHandle()->setLinearFactor(ToBulletVector3(factor));
		}

		void RigidBody::SetAngularAirResistance(float value)
		{
			rigidbody->GetNativeHandle()->setDamping(this->GetLinearAirResistance(), value);
		}

		float RigidBody::GetAngularAirResistance() const
		{
			return rigidbody->GetNativeHandle()->getAngularDamping();;
		}

		void RigidBody::SetLinearAirResistance(float value)
		{
			rigidbody->GetNativeHandle()->setDamping(value, this->GetAngularAirResistance());
		}

		float RigidBody::GetLinearAirResistance() const
		{
			return rigidbody->GetNativeHandle()->getLinearDamping();
		}

		Vector3 RigidBody::GetTotalForceApplied() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTotalForce());
		}

		Vector3 RigidBody::GetTotalTorqueApplied() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTotalTorque());
		}

		Vector3 RigidBody::GetInertia() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getLocalInertia());
		}

		Vector3 RigidBody::GetVelocityInPoint(Vector3 relativePosition)
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getVelocityInLocalPoint(ToBulletVector3(relativePosition)));
		}

		Vector3 RigidBody::GetPushVelocityInPoint(Vector3 relativePosition)
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getPushVelocityInLocalPoint(ToBulletVector3(relativePosition)));
		}

		void RigidBody::ApplyCentralImpulse(Vector3 impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyCentralPushImpulse(Vector3 impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralPushImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyForce(const Vector3& force, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyForce(ToBulletVector3(force), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyPushImpulse(ToBulletVector3(impulse), ToBulletVector3(relativePosition));
		}

		void RigidBody::ApplyTorque(const Vector3& force)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorque(ToBulletVector3(force));
		}

		void RigidBody::ApplyTorqueImpulse(const Vector3& impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorqueImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyTorqueTurnImpulse(const Vector3& impulse)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyTorqueTurnImpulse(ToBulletVector3(impulse));
		}

		void RigidBody::ApplyCentralForce(const Vector3& force)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->applyCentralForce(ToBulletVector3(force));
		}

		void RigidBody::SetPushVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setPushVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody::GetPushVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getPushVelocity());
		}

		void RigidBody::SetTurnVelocity(Vector3 velocity)
		{
			rigidbody->Activate();
			rigidbody->GetNativeHandle()->setTurnVelocity(ToBulletVector3(velocity));
		}

		Vector3 RigidBody::GetTurnVelocity() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getTurnVelocity());
		}

		void RigidBody::SetAnisotropicFriction(Vector3 friction, AnisotropicFriction mode)
		{
			rigidbody->GetNativeHandle()->setAnisotropicFriction(ToBulletVector3(friction), (int)mode);
		}

		void RigidBody::SetAnisotropicFriction(Vector3 friction)
		{
			SetAnisotropicFriction(friction, HasAnisotropicFriction() ? AnisotropicFriction::ENABLED : AnisotropicFriction::DISABLED);
		}

		bool RigidBody::HasAnisotropicFriction() const
		{
			return rigidbody->GetNativeHandle()->hasAnisotropicFriction();
		}

		void RigidBody::ToggleAnisotropicFriction(bool value)
		{
			SetAnisotropicFriction(GetAnisotropicFriction(), value ? AnisotropicFriction::ENABLED : AnisotropicFriction::DISABLED);
		}

		Vector3 RigidBody::GetAnisotropicFriction() const
		{
			return FromBulletVector3(rigidbody->GetNativeHandle()->getAnisotropicFriction());
		}
	}
}