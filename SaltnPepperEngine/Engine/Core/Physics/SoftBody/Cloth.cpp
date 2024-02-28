#include "Cloth.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;
}

	/*namespace Verlet
	{
		ClothPoint::ClothPoint(const Vector3& newPosition)
		{
			position = newPosition;
			prevPosition = newPosition;

			velocity = Vector3(0.0f);
			isPinned = false;
		}

		ClothPoint::~ClothPoint()
		{
		}

		void ClothPoint::OnUpdate(const float& fixedDeltaTime)
		{
			if (isPinned) { return; }

			AddForce(windForce + gravity);

			Vector3 newPos = position + (position - prevPosition) * friction + velocity * fixedDeltaTime * fixedDeltaTime;


			prevPosition = position;

			position = newPos;
			velocity = Vector3(0.0f);



			if()


			if (this.x >= 1) {
				this.px = 1 + (1 - this.px) * bounce;
				this.x = 1;
			}

			if (this.y >= 1) {
				this.py = 1 + (1 - this.py) * bounce;
				this.y = 1;
			}
			else if (this.y <= -1.0) {
				this.py *= -1.0 * bounce;
				this.y = -1.0;
			}


		}

		void ClothPoint::Resolve()
		{
		}
		void ClothPoint::Attach(const ClothPoint& point)
		{
		}
		void ClothPoint::Free()
		{
		}
		void ClothPoint::AddForce(const Vector3& newForce)
		{
		}
		void ClothPoint::Pin()
		{
		}


		ClothConstraint::ClothConstraint(ClothPoint* p1, ClothPoint* p2)
		{
			pointOne = p1;
			pointTwo = p2;

			length = Distance(pointOne->position, pointTwo->position);
		}

		ClothConstraint::~ClothConstraint()
		{

		}

		void ClothConstraint::Resolve()
		{
			float distance = Distance(pointOne->position, pointTwo->position);
			
			if (distance < length) return;


			float difference = (length - distance) / distance;

			float multiplier = difference * 0.5f * (1.0f - length / distance);

			Vector3 newPosition = (pointOne->position - pointTwo->position) * multiplier;

			if (!pointOne->isPinned)
			{
				pointOne->position += newPosition;
			}
			if (!pointTwo->isPinned)
			{
				pointTwo->position -= newPosition;
			}
		}
	}*/
