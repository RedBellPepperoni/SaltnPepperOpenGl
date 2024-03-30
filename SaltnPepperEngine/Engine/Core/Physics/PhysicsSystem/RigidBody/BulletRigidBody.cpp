#include "BulletRigidBody.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsUtils.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		/*class MotionStateNotifier : public btDefaultMotionState
		{
		public:
			MotionStateNotifier(btTransform& tr) : btDefaultMotionState(tr) { }

			bool TransformUpdated = true;

			virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
			{
				this->TransformUpdated = true;
				btDefaultMotionState::setWorldTransform(centerOfMassWorldTrans);
			}
		};*/

		/*struct BulletRigidBody::RigidBodyAllocation
		{
			btRigidBody* Body = nullptr;
			MotionStateNotifier* MotionState = nullptr;
		};*/

		
		void BulletRigidBody::DestroyBody()
		{
			//if (this->m_bodyAllocation != nullptr)
			if (Body != nullptr)
			{
				btRigidBody* body = Body;

				PhysicsUtils::ActiveRigidBodyIsland(body);
				PhysicsUtils::RemoveRigidBody(body);

				MotionState->~MotionStateNotifier();
				body->~btRigidBody();
	
				//std::free((void*)this->m_bodyAllocation);
			}
		}


		void BulletRigidBody::ReAddRigidBody()
		{
			auto rigid = GetNativeHandle();

			PhysicsUtils::RemoveRigidBody(this->GetNativeHandle());

			auto rigid2 = GetNativeHandle();

			PhysicsUtils::AddRigidBody(this->GetNativeHandle(), m_group, m_layer);
		}

		void BulletRigidBody::UpdateCollider(float mass, btCollisionShape* collider)
		{
			btVector3 inertia(0.0f, 0.0f, 0.0f);

			if (mass == 0.0f) { mass = 1.0f; }

			if (collider != nullptr && mass != 0.0f) 
			{
				collider->calculateLocalInertia(mass, inertia);
			}

			this->GetNativeHandle()->setMassProps(mass, inertia);

			if (collider != this->GetCollisionShape()) 
			{
				this->GetNativeHandle()->setCollisionShape(collider); 
				this->ReAddRigidBody(); 
			}
		}

		BulletRigidBody::BulletRigidBody(const Transform& transform)
		{
			btTransform bulletTransform;

			ToBulletTransform(transform, bulletTransform);

			/*m_bodyAllocation = reinterpret_cast<RigidBodyAllocation*>(std::malloc(sizeof(RigidBodyAllocation)));
			auto state = new((uint8_t*)m_bodyAllocation + sizeof(btRigidBody)) MotionStateNotifier(bulletTransform);
			auto body = new(m_bodyAllocation) btRigidBody(0.0f, state, nullptr, btVector3(0, 0, 0));*/

			//m_bodyAllocation = MakeShared<RigidBodyAllocation>();
			MotionState = new MotionStateNotifier(bulletTransform);

			btRigidBody::btRigidBodyConstructionInfo info(10, MotionState, nullptr, btVector3(0, 0, 0));


			Body = new btRigidBody(info);

			PhysicsUtils::AddRigidBody(Body, m_group, m_layer);
		}

		BulletRigidBody::BulletRigidBody(const Transform& transform, btCollisionShape* collider)
		{
			btTransform bulletTransform;

			ToBulletTransform(transform, bulletTransform);
			MotionState = new MotionStateNotifier(bulletTransform);

			btRigidBody::btRigidBodyConstructionInfo info(10,MotionState,collider,btVector3(0,0,0));

			Body = new btRigidBody(info);

			PhysicsUtils::AddRigidBody(Body, m_group, m_layer);
		}

		BulletRigidBody::BulletRigidBody(BulletRigidBody&& other) noexcept
		{
			/*m_bodyAllocation = other.m_bodyAllocation;
			other.m_bodyAllocation = nullptr;*/

			Body = other.Body;
			other.Body = nullptr;

			MotionState = other.MotionState;
			other.MotionState = nullptr;
		}

		BulletRigidBody& BulletRigidBody::operator=(BulletRigidBody&& other) noexcept
		{
			DestroyBody();
			/*m_bodyAllocation = other.m_bodyAllocation;
			other.m_bodyAllocation = nullptr;*/

			Body = other.Body;
			other.Body = nullptr;

			MotionState = other.MotionState;
			other.MotionState = nullptr;
			return *this;
		}

		BulletRigidBody::~BulletRigidBody()
		{
			DestroyBody();
		}

		btRigidBody* BulletRigidBody::GetNativeHandle()
		{
			//return std::addressof(m_bodyAllocation->Body);
			//return m_bodyAllocation->Body;
			return Body;
		}

		const btRigidBody* BulletRigidBody::GetNativeHandle() const
		{
			//return std::addressof(m_bodyAllocation->Body);
			return Body;
		}

		btMotionState* BulletRigidBody::GetMotionState()
		{
			return GetNativeHandle()->getMotionState();
		}

		const btMotionState* BulletRigidBody::GetMotionState() const
		{
			return GetNativeHandle()->getMotionState();
		}

		bool BulletRigidBody::HasTransformUpdate() const
		{
			return static_cast<const MotionStateNotifier*>(this->GetMotionState())->TransformUpdated;
		}

		void BulletRigidBody::SetTransformUpdateFlag(bool value)
		{
			static_cast<MotionStateNotifier*>(this->GetMotionState())->TransformUpdated = value;
		}

		btCollisionShape* BulletRigidBody::GetCollisionShape()
		{
			return GetNativeHandle()->getCollisionShape();
		}
		const btCollisionShape* BulletRigidBody::GetCollisionShape() const
		{
			return GetNativeHandle()->getCollisionShape();
		}

		void BulletRigidBody::SetCollisionShape(btCollisionShape* shape)
		{
			UpdateCollider(this->GetMass(), shape);
		}

		void BulletRigidBody::SetCollisionFilter(uint32_t group, uint32_t layer)
		{
			m_layer = layer;
			m_group = group;
			ReAddRigidBody();
		}

		uint32_t BulletRigidBody::GetCollisionGroup() const
		{
			return m_group;
		}
		uint32_t BulletRigidBody::GetCollisionMask() const
		{
			return m_layer;
		}
		Vector3 BulletRigidBody::GetScale() const
		{
			const btCollisionShape* collider = GetCollisionShape();

			if (collider != nullptr)
			{
				return FromBulletVector3(collider->getLocalScaling());
			}
			else
			{
				return Vector3{0.0f};

			}
		}

		bool BulletRigidBody::IsMoving() const
		{
			const btCollisionShape* collider = this->GetCollisionShape();
			
			return collider != nullptr && !collider->isNonMoving();
		}


#undef DISABLE_DEACTIVATION
#undef ACTIVE_TAG


		bool BulletRigidBody::HasCollisionResponce() const
		{
			return !(GetNativeHandle()->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		void BulletRigidBody::SetKinematicFlag()
		{
			btRigidBody* body = GetNativeHandle();

			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			SetActivationState(ActivationState::DISABLEDEACTIVATION);
		}

		void BulletRigidBody::UnsetKinematicFlag()
		{
			btRigidBody* body = GetNativeHandle();
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
			SetActivationState(ActivationState::ACTIVETAG);
		}

		void BulletRigidBody::SetTriggerFlag()
		{
			btRigidBody* body = GetNativeHandle();
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		void BulletRigidBody::UnsetTriggerFlag()
		{
			btRigidBody* body = GetNativeHandle();
			body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		}

		void BulletRigidBody::UnsetAllFlags()
		{
			UnsetTriggerFlag();
			UnsetKinematicFlag();
		}

		void BulletRigidBody::SetScale(const Vector3& scale)
		{
			btCollisionShape* collider = this->GetCollisionShape();
			if (collider != nullptr)
			{
				collider->setLocalScaling(ToBulletVector3(scale));
			}
		}

		float BulletRigidBody::GetMass() const
		{
			return GetNativeHandle()->getMass();
		}

		void BulletRigidBody::SetMass(float mass)
		{
			UpdateCollider(mass, this->GetCollisionShape());
		}

		void BulletRigidBody::SetActivationState(ActivationState state)
		{
			GetNativeHandle()->forceActivationState((int)state);
		}

		ActivationState BulletRigidBody::GetActivationState() const
		{
			return ActivationState(this->GetNativeHandle()->getActivationState());
		}

		void BulletRigidBody::Activate()
		{
			GetNativeHandle()->activate(true);
		}

		bool BulletRigidBody::IsActive() const
		{
			return GetNativeHandle()->isActive();
		}
	}
}