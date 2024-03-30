#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include"Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsSystem.h"

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

		class RigidBody
		{
		private:

			btRigidBody* bodyHandle = nullptr;
			btMotionState* motionHandle = nullptr;

		public:

			RigidBody(const Vector3& position, btCollisionShape* shape)
			{
				btVector3 pos = ToBulletVector3(position);

				motionHandle = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
					pos));

				btRigidBody::btRigidBodyConstructionInfo info(10, motionHandle, shape, btVector3(0, 0, 0));
				bodyHandle = new btRigidBody(info);

				PhysicsSystem::GetCurrent()->World->addRigidBody(bodyHandle);
			}
			~RigidBody()
			{
				delete motionHandle;
				delete bodyHandle;
			};

			btRigidBody* GetBody() { return bodyHandle; }

		};

	}
}

#endif // !RIGIDBODY_H
