#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class BoxCollider : public BaseCollider
		{
			SharedPtr<BoxShape> boxShape;
			void CreateNewShape(const BoundingBox& box);

		public:
			BoxCollider() = default;
			void Init(BoundingBox box);
			void UpdateCollider(BoundingBox box);

			virtual ~BoxCollider() = default;
		
			BoxShape* GetNativeHandle() const;
			virtual btCollisionShape* GetShape() override { return boxShape->GetNativeHandle(); }
		
			virtual void DebugDraw(Transform transform) override;

			BoundingBox GetAABB(Transform& transform) const;
			BoundingBox GetOBB(Transform& transform) const;
			BoundingBox GetOBBInternal() const;
			BoundingSphere GetBoundingSphere(const Transform& transform) const;

			void SetBoundingBox(BoundingBox box);


		};

	}
}

#endif // !BOXCOLLIDER_H

