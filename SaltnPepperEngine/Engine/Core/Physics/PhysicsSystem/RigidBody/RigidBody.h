#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include"Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsSystem.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

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

			SharedPtr<btRigidBody> bodyHandle = nullptr;
			//btMotionState* motionHandle = nullptr;
			SharedPtr<MotionStateNotifier> motionHandle = nullptr;

		public:

			RigidBody(const Vector3& position, btCollisionShape* shape)
			{
				btVector3 pos = ToBulletVector3(position);
				btTransform transform = btTransform(btQuaternion(0, 0, 0, 1), pos);
				motionHandle = MakeShared<MotionStateNotifier>(transform);

				btRigidBody::btRigidBodyConstructionInfo info(10, motionHandle.get(), shape, btVector3(0, 0, 0));
				//bodyHandle = new btRigidBody(info);
				bodyHandle = MakeShared<btRigidBody>(info);

				PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle());

			}
			~RigidBody()
			{
				//delete motionHandle;
				//delete bodyHandle;
			};

			btRigidBody* GetNativeHandle() { return bodyHandle.get(); }

		};

	}
}

#endif // !RIGIDBODY_H
