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

			PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle(), collisionGroup, collisionMask);


		}

		RigidBody::~RigidBody()
		{
			DestroyBody();
		}

		void RigidBody::SetMass(const float value)
		{
			UpdateCollider(value, shapeHandle);
		}

		const float RigidBody::GetMass() const
		{
			return static_cast<float>(bodyHandle->getMass());
		}
	}
}