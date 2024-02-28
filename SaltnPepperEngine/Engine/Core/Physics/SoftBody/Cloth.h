#ifndef CLOTH_H
#define CLOTH_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{
	/*namespace Components
	{
		class Transform;
	}

	namespace Rendering
	{
		class Mesh;
		struct Vertex;
	}

	using Rendering::Mesh;
	using Components::Transform;

	
	namespace Verlet
	{*/
		/*struct ClothPoint;


		static Vector3 gravity = Vector3(0.0f, -10.0f, 0.0f);
		static Vector3 windForce = Vector3(0.0f);
		static float friction = 0.99f

		struct ClothConstraint
		{
			ClothConstraint(ClothPoint* p1, ClothPoint* p2);
			~ClothConstraint();

			ClothPoint* pointOne = nullptr;
			ClothPoint* pointTwo = nullptr;
			float length = 0.0f;

			void Resolve();
		};

		struct ClothPoint
		{
			Vector3 position = Vector3(0.0f);
			Vector3 prevPosition = Vector3(0.0f);

			Vector3 velocity = Vector3(0.0f);

			bool isPinned = false;

			ClothPoint(const Vector3& newPosition);
			~ClothPoint();

			void OnUpdate(const float& fixedDeltaTime);
			void Resolve();
			void Attach(const ClothPoint& point);
			void Free();

			void AddForce(const Vector3& newForce);
			void Pin();
		};


	}*/
	
}

#endif // !CLOTH_H

