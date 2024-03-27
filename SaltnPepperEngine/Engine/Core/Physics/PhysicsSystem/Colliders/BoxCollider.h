#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class BoxCollider : BaseCollider
		{
			SharedPtr<BoxShape> boxShape;
			void CreateNewShape(const OrientedBoundingBox& box);

		public:
			BoxCollider() = default;
			void Init(BoundingBox box);
			void UpdateCollider(BoundingBox box);

			virtual ~BoxCollider() = default;
		
			BoxShape* GetNativeHandle() const;
		
			BoundingBox GetAABB(const Transform& transform) const;
			OrientedBoundingBox GetOBB(const Transform& transform) const;
			OrientedBoundingBox GetOBBInternal() const;
			BoundingSphere GetBoundingSphere(const Transform& transform) const;

			void SetBoundingBox(OrientedBoundingBox box);


		};

	}
}

#endif // !BOXCOLLIDER_H

