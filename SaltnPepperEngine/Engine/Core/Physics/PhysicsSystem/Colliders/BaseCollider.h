#ifndef BASECOLLIDER_H
#define BASECOLLIDER_H

#include "Engine/Core/Memory/MemoryDefinitions.h"

class btCollisionShape;

namespace SaltnPepperEngine
{
	class BoundingSphere;
	class BoundingBox;

	namespace Rendering
	{
		class Mesh;
	}

	using Rendering::Mesh;

	namespace Physics
	{
		//Can be used as MeshCollider later
		class BaseCollider
		{
			bool colliderChangedFlag = true;
	

		protected:
			
			

		public:

			virtual btCollisionShape* GetShape() = 0;

			virtual ~BaseCollider() = default;

			void SetColliderChangedFlag(bool value) { colliderChangedFlag = value; }
			bool HasColliderChanged() const { return colliderChangedFlag; }

		};

	}
}

#endif // !BASECOLLIDER_H


