#ifndef BULLET3BINDINGS_H
#define BULLET3BINDINGS_H

#include <btBulletDynamicsCommon.h>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Components/Transform.h"

namespace SaltnPepperEngine
{
	using Components::Transform;

	namespace Physics
	{
		inline btVector3 ToBulletVector3(const Vector3& engineVector) { return btVector3(engineVector.x, engineVector.y, engineVector.z); }
	
		inline Vector3 FromBulletVector3(const btVector3& bulletVector) { return Vector3{bulletVector.x(),bulletVector.y(),bulletVector.z()}; }
	
		inline void ToBulletTransform(const Transform& engineTransform, btTransform& bulletTransform)
		{
			Quaternion rot = engineTransform.GetRotation();

			bulletTransform.setIdentity();
			bulletTransform.setOrigin(ToBulletVector3(engineTransform.GetPosition()));
			bulletTransform.setRotation(*reinterpret_cast<btQuaternion*>(&rot));
		}
	
		inline void FromBulletTransform(const btTransform& bulletTransform, Transform& engineTransform)
		{
			btQuaternion rot = bulletTransform.getRotation();
			Vector3 position = FromBulletVector3(bulletTransform.getOrigin());

			engineTransform.SetPosition(position);
//			engineTransform.SetRotation(*reinterpret_cast<Quaternion*>(&rot));
		}
	}
}

#endif // !BULLET3BINDINGS_H
