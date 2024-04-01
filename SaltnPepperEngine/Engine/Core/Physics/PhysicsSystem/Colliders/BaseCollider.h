#ifndef BASECOLLIDER_H
#define BASECOLLIDER_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/Transform.h"

class btCollisionShape;

namespace SaltnPepperEngine
{
	class BoundingSphere;
	class BoundingBox;

	using namespace Components;

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

			virtual void DebugDraw(const Transform& transform) {};

		};

	}
}

#endif // !BASECOLLIDER_H


