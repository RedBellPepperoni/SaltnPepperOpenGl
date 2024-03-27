#include "RigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"

#include "Engine/Core/Physics/PhysicsSystem/Colliders/BoxCollider.h"
//#include "Engine/Core/Physics/PhysicsSystem/Colliders/SphereCollider.h"
//#include "Engine/Core/Physics/PhysicsSystem/Colliders/CapsuleCollider.h"

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
				FromBulletTransform(rigidbody->GetNativeHandle()->getWorldTransform(), ECSTransform);
				rigidbody->SetTransformUpdateFlag(false);

				localTransform = ECSTransform;

			}


			if (selfScale != rigidbody->GetScale())
			{
				rigidbody->SetScale(selfScale);
				localTransform.SetScale(selfScale);
			}

		}

		

		void RigidBody::UpdateCollider(BaseCollider* Collider)
		{
			if (Collider == nullptr)
			{
				rigidbody->SetCollisionShape(nullptr);
			}

			//if (TestCollider(rigidbody, static_cast<BoxCollider>(Collider)));
		}

		void RigidBody::Init()
		{
		}

		void RigidBody::OnUpdate(float dt)
		{
		}

		void RigidBody::SyncObjectState()
		{
		}

		BulletRigidBody* RigidBody::GetBulletHandle() const
		{
			return nullptr;
		}

		void RigidBody::InvokeOnCollisionCallback(RigidBody* self, RigidBody* object)
		{
		}

		void RigidBody::InvokeOnCollisionEnterCallback(RigidBody* self, RigidBody* object)
		{
		}

		void RigidBody::InvokeOnCollisionExitCallback(RigidBody* self, RigidBody* object)
		{
		}

		void RigidBody::MakeKinematic()
		{
		}

		void RigidBody::MakeDynamic()
		{
		}

		void RigidBody::MakeStatic()
		{
		}

		void RigidBody::MakeTrigger()
		{
		}

		bool RigidBody::IsKinematic() const
		{
			return false;
		}

		bool RigidBody::IsDynamic() const
		{
			return false;
		}

		bool RigidBody::IsStatic() const
		{
			return false;
		}

		bool RigidBody::IsTrigger() const
		{
			return false;
		}

		bool RigidBody::IsRayCastable() const
		{
			return false;
		}

		void RigidBody::ToggleRayCasting(bool value)
		{
		}

		bool RigidBody::IsMoving() const
		{
			return false;
		}

		void RigidBody::ToggleTrigger(bool value)
		{
		}

		RigidBodyType RigidBody::GetTypeInternal() const
		{
			return RigidBodyType();
		}

		void RigidBody::SetTypeInternal(RigidBodyType type)
		{
		}

		void RigidBody::SetCollisionFilter(uint32_t mask, uint32_t group)
		{
		}

		void RigidBody::SetCollisionFilter(CollisionLayer::Mask mask, CollisionGroup::Group group)
		{
		}

		uint32_t RigidBody::GetCollisionGroup() const
		{
			return 0;
		}

		uint32_t RigidBody::GetCollisionMask() const
		{
			return 0;
		}
		void RigidBody::ActivateParentIsland()
		{
		}

		void RigidBody::SetActivationState(ActivationState state)
		{
		}

		ActivationState RigidBody::GetActivationState() const
		{
			return ActivationState();
		}

		BoundingBox RigidBody::GetAABB() const
		{
			return BoundingBox();
		}

		void RigidBody::Activate()
		{
		}

		void RigidBody::ClearForces()
		{
		}

		float RigidBody::GetMass() const
		{
			return 0.0f;
		}

		void RigidBody::SetMass(float mass)
		{
		}

		float RigidBody::GetFriction() const
		{
			return 0.0f;
		}

		void RigidBody::SetFriction(float value)
		{

		}

		float RigidBody::GetSpinningFriction() const
		{
			return 0.0f;
		}

		void RigidBody::SetSpinningFriction(float value)
		{
		}

		float RigidBody::GetRollingFriction() const
		{
			return 0.0f;
		}

		void RigidBody::SetRollingFriction(float value)
		{
		}

		float RigidBody::GetBounceFactor() const
		{
			return 0.0f;
		}
		void RigidBody::SetBounceFactor(float value)
		{
		}

		void RigidBody::SetGravity(Vector3 gravity)
		{
		}

		Vector3 RigidBody::GetGravity() const
		{
			return Vector3();
		}

		void RigidBody::SetLinearVelocity(Vector3 velocity)
		{
		}

		Vector3 RigidBody::GetLinearVelocity() const
		{
			return Vector3();
		}

		void RigidBody::SetAngularVelocity(Vector3 velocity)
		{
		}

		Vector3 RigidBody::GetAngularVelocity() const
		{
			return Vector3();
		}

		float RigidBody::GetInverseMass() const
		{
			return 0.0f;
		}

		Vector3 RigidBody::GetAngularForceFactor() const
		{
			return Vector3();
		}

		void RigidBody::SetAngularForceFactor(Vector3 factor)
		{
		}

		Vector3 RigidBody::GetLinearForceFactor() const
		{
			return Vector3();
		}

		void RigidBody::SetLinearForceFactor(Vector3 factor)
		{
		}

		void RigidBody::SetAngularAirResistance(float value)
		{

		}

		float RigidBody::GetAngularAirResistance() const
		{
			return 0.0f;
		}

		void RigidBody::SetLinearAirResistance(float value)
		{
		}

		float RigidBody::GetLinearAirResistance() const
		{
			return 0.0f;
		}

		Vector3 RigidBody::GetTotalForceApplied() const
		{
			return Vector3();
		}

		Vector3 RigidBody::GetTotalTorqueApplied() const
		{
			return Vector3();
		}

		Vector3 RigidBody::GetInertia() const
		{
			return Vector3();
		}

		Vector3 RigidBody::GetVelocityInPoint(Vector3 relativePosition)
		{
			return Vector3();
		}

		Vector3 RigidBody::GetPushVelocityInPoint(Vector3 relativePosition)
		{
			return Vector3();
		}

		void RigidBody::ApplyCentralImpulse(Vector3 impulse)
		{
		}

		void RigidBody::ApplyCentralPushImpulse(Vector3 impulse)
		{
		}

		void RigidBody::ApplyForce(const Vector3& force, const Vector3& relativePosition)
		{
		}

		void RigidBody::ApplyImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
		}

		void RigidBody::ApplyPushImpulse(const Vector3& impulse, const Vector3& relativePosition)
		{
		}

		void RigidBody::ApplyTorque(const Vector3& force)
		{
		}

		void RigidBody::ApplyTorqueImpulse(const Vector3& impulse)
		{
		}

		void RigidBody::ApplyTorqueTurnImpulse(const Vector3& impulse)
		{
		}

		void RigidBody::ApplyCentralForce(const Vector3& force)
		{
		}

		void RigidBody::SetPushVelocity(Vector3 velocity)
		{
		}

		Vector3 RigidBody::GetPushVelocity() const
		{
			return Vector3();
		}

		void RigidBody::SetTurnVelocity(Vector3 velocity)
		{
		}

		Vector3 RigidBody::GetTurnVelocity() const
		{
			return Vector3();
		}

		void RigidBody::SetAnisotropicFriction(Vector3 friction, AnisotropicFriction mode)
		{
		}

		void RigidBody::SetAnisotropicFriction(Vector3 friction)
		{
		}

		bool RigidBody::HasAnisotropicFriction() const
		{
			return false;
		}

		void RigidBody::ToggleAnisotropicFriction(bool value)
		{
		}

		Vector3 RigidBody::GetAnisotropicFriction() const
		{
			return Vector3();
		}
	}
}