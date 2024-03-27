#ifndef BASESHAPE_H
#define BASESHAPE_H

#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Physics/PhysicsSystem/Bounding/AABB.h"

#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingSphere.h"
#include "Engine/Core/Physics/Collision/BoundingStuff/BoundingBox.h"
#include "Engine/Core/Components/Transform.h"

class btCollisionShape;

namespace SaltnPepperEngine
{

	using Components::Transform;

	namespace Physics
	{

		class BaseShape
		{

		protected:

			btCollisionShape* collider = nullptr;

			void DestroyShape();

			/*BoundingSphere boundingsphere;
			BoundingBox boundingbox;*/

			template<typename T, typename... Args>
			T* CreateShape(Args&&... args)
			{
				this->collider = new T(std::forward<Args>(args)...);
				return static_cast<T*>(this->collider);
			}

		public:

			void SetScale(const Vector3& scale);
			const Vector3 GetScale() const;

			btCollisionShape* GetHandle();
			
			BoundingBox GetAABB() const;
			BoundingBox GetAABBTranformed(const Transform& transform) const;

			BoundingSphere GetBoundingSphere() const;
			BoundingSphere GetBoundingSphereTransformed(const Transform& transform) const;
		};
	
	}
}

#endif // !BASESHAPE_H
