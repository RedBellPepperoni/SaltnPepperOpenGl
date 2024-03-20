#include "BulletRigidBody.h"
#include "Bullet3Bindings.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
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

		struct BulletRigidBody::RigidBodyAllocation
		{
			btRigidBody Body;
			MotionStateNotifier MotionState;
		};

		
		void BulletRigidBody::DestroyBody()
		{
			if (this->m_bodyAllocation != nullptr)
			{
				btRigidBody* body = std::addressof(this->m_bodyAllocation->Body);

				/*Physics::ActiveRigidBodyIsland(body);
				Physics::RemoveRigidBody(body);

				this->bodyAllocation->MotionState.~MotionStateNotifier();
				body->~btRigidBody();

				std::free((void*)this->bodyAllocation);*/
			}
		}


		void BulletRigidBody::ReAddRigidBody()
		{
		}
		void BulletRigidBody::UpdateCollider(float mass, btCollisionShape* collider)
		{
		}
		BulletRigidBody::BulletRigidBody(const Transform& transform)
		{
		}
		BulletRigidBody::BulletRigidBody(BulletRigidBody&&) noexcept
		{
		}
		//BulletRigidBody& BulletRigidBody::operator=(BulletRigidBody&&) noexcept
		//{
		//	// TODO: insert return statement here
		//}
		BulletRigidBody::~BulletRigidBody()
		{
		}
		btRigidBody* BulletRigidBody::GetNativeHandle()
		{
			return nullptr;
		}
		const btRigidBody* BulletRigidBody::GetNativeHandle() const
		{
			return nullptr;
		}
		btMotionState* BulletRigidBody::GetMotionState()
		{
			return nullptr;
		}
		const btMotionState* BulletRigidBody::GetMotionState() const
		{
			return nullptr;
		}
		bool BulletRigidBody::HasTransformUpdate() const
		{
			return false;
		}
		void BulletRigidBody::SetTransformUpdateFlag(bool value)
		{
		}
		btCollisionShape* BulletRigidBody::GetCollisionShape()
		{
			return nullptr;
		}
		const btCollisionShape* BulletRigidBody::GetCollisionShape() const
		{
			return nullptr;
		}
		void BulletRigidBody::SetCollisionShape(btCollisionShape* shape)
		{
		}
		void BulletRigidBody::SetCollisionFilter(uint32_t group, uint32_t layer)
		{
		}
		uint32_t BulletRigidBody::GetCollisionGroup() const
		{
			return 0;
		}
		uint32_t BulletRigidBody::GetCollisionMask() const
		{
			return 0;
		}
		Vector3 BulletRigidBody::GetScale() const
		{
			return Vector3();
		}
		bool BulletRigidBody::IsMoving() const
		{
			return false;
		}
		bool BulletRigidBody::HasCollisionResponce() const
		{
			return false;
		}
		void BulletRigidBody::SetKinematicFlag()
		{
		}
		void BulletRigidBody::UnsetKinematicFlag()
		{
		}
		void BulletRigidBody::SetTriggerFlag()
		{
		}
		void BulletRigidBody::UnsetTriggerFlag()
		{
		}
		void BulletRigidBody::UnsetAllFlags()
		{
		}
		void BulletRigidBody::SetScale(const Vector3& scale)
		{
		}
		float BulletRigidBody::GetMass() const
		{
			return 0.0f;
		}
		void BulletRigidBody::SetMass(float mass)
		{
		}
		void BulletRigidBody::SetActivationState(ActivationState state)
		{
		}
		ActivationState BulletRigidBody::GetActivationState() const
		{
			return ActivationState();
		}
		void BulletRigidBody::Activate()
		{
		}
		bool BulletRigidBody::IsActive() const
		{
			return false;
		}
	}
}