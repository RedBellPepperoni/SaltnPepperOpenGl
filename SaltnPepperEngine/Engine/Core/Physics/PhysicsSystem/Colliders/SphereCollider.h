#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H


#include "BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"

namespace SaltnPepperEngine
{
	namespace Physics
	{
		class SphereCollider : public BaseCollider
		{
			SharedPtr<SphereShape> sphereShape;
            void CreateNewShape(const BoundingSphere& sphere);
        public:
            SphereCollider() = default;
            void Init(const BoundingSphere& sphere);
            void UpdateCollider(const BoundingSphere& sphere);

            virtual btCollisionShape* GetShape() override { return sphereShape->GetNativeHandle(); }

            virtual void DebugDraw(Transform transform) override ;

            SphereShape* GetNativeHandle() const;

            BoundingBox GetAABB(const Transform& transform) const;
            BoundingSphere GetBoundingSphere(const Transform& transform) const;
            BoundingSphere GetBoundingSphereInternal() const;
            void SetBoundingSphere(BoundingSphere sphere);
		};
	}
}

#endif // !SPHERECOLLIDER_H
