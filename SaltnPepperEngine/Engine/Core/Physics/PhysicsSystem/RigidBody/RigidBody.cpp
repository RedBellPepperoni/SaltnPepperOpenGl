#include "RigidBody.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
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

			PhysicsSystem::GetCurrent()->World->addRigidBody(GetNativeHandle());


		}

		const float RigidBody::GetMass() const
		{
			return static_cast<float>(bodyHandle->getMass());
		}
	}
}