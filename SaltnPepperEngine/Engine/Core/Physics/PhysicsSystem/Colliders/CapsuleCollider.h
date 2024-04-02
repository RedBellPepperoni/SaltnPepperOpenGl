#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H


#include "BaseCollider.h"
#include "Engine/Core/Physics/PhysicsSystem/PhysicsAPI.h"


namespace SaltnPepperEngine
{
	namespace Physics
	{
		class CapsuleCollider : public BaseCollider
		{
			SharedPtr<CapsuleShape> capsuleShape;

            void CreateNewShape(const BoundingCapsule& capsule);
        public:
            CapsuleCollider() = default;
            void Init(const BoundingCapsule& capsule);
            void UpdateCollider(const BoundingCapsule& capsule);

            CapsuleShape* GetNativeHandle() const;

            virtual btCollisionShape* GetShape() override { return capsuleShape->GetNativeHandle(); }
            virtual void DebugDraw(Transform transform) override;

            void SetOrientation(const BoundingBox& aabb, BoundingCapsule::Axis axis);
            BoundingCapsule::Axis GetOrientation() const;

            BoundingBox GetAABB(const Transform& transform) const;
            BoundingSphere GetBoundingSphere(const Transform& transform) const;
            BoundingCapsule GetBoundingCapsule(const Transform& transform) const;
            BoundingCapsule GetBoundingCapsuleInternal() const;
            void SetBoundingCapsule(BoundingCapsule capsule);

		};
	}
}

#endif // !CAPSULECOLLIDER_H
