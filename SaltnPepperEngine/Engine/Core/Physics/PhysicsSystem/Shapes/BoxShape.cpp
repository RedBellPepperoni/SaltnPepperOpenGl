#include "BoxShape.h"
#include "Engine/Core/Physics/PhysicsSystem/Bullet3Bindings.h"


namespace SaltnPepperEngine
{

	namespace Physics
	{
		BoxShape::BoxShape(const BoundingBox& boundingBox)
		{
			box = boundingBox;
			CreateShape<btBoxShape>(ToBulletVector3(boundingBox.Length() * 0.5f));
		}

		BoxShape::BoxShape(BoxShape&& other) noexcept
		{
			collider = other.collider;
			other.collider = nullptr;
			box = other.box;
		}

		BoxShape& BoxShape::operator=(BoxShape&& other) noexcept
		{
			DestroyShape();
			collider = other.collider;
			other.collider = nullptr;
			box = other.box;
			return *this;
		}

		BoxShape::~BoxShape()
		{
			DestroyShape();
		}

		BoundingBox BoxShape::GetBoundingBoxTransformed(Transform& transform) const
		{
			

			BoundingBox resultbox = this->GetBoundingBox();
			BoundingBox finalBox;
			finalBox = resultbox.GetTransformedBox(transform.GetLocalMatrix());

			return finalBox;
			
		}

		BoundingBox BoxShape::GetBoundingBox() const
		{		
			return box;
		}

		BoundingBox BoxShape::GetNativeBounding() const
		{
			return GetAABB();
		}

		BoundingBox BoxShape::GetNativeBoundingTransformed(Transform& transform) const
		{
			BoundingBox result = GetNativeBounding();

			return result.GetTransformedBox(transform.GetLocalMatrix());
		}
	}


}
