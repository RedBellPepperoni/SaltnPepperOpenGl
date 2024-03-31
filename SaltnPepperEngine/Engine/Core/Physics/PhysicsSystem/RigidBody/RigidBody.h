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

			btCollisionShape* shapeHandle = nullptr;

		public:

			RigidBody(const Vector3& position, btCollisionShape* shape);
			
			~RigidBody()
			{
				//delete motionHandle;
				//delete bodyHandle;
			};

			btRigidBody* GetNativeHandle() { return bodyHandle.get(); }
			const btRigidBody* GetNativeHandle() const { return bodyHandle.get(); }

			MotionStateNotifier* GetMotionState() { return motionHandle.get(); }
			const MotionStateNotifier* GetMotionState() const { return motionHandle.get(); }


			//void SetMass(const float value);
			const float GetMass() const;
		};

	}
}

#endif // !RIGIDBODY_H
