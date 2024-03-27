#ifndef BASECOLLIDER_H
#define BASECOLLIDER_H

#include "Engine/Core/Memory/MemoryDefinitions.h"


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
			void SetColliderChangedFlag(bool value) { colliderChangedFlag = value; }
			bool HasColliderChanged() const { return colliderChangedFlag; }

		};

	}
}

#endif // !BASECOLLIDER_H


