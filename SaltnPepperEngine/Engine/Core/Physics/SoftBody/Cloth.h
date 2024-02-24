#ifndef CLOTH_H
#define CLOTH_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class Mesh;
	}




	using Rendering::Mesh;

	namespace Physics
	{

		class SoftBodyParticle
		{
		public:

			SoftBodyParticle()
			{
			}

			~SoftBodyParticle()
			{}

			SoftBodyParticle(Vector3 NewPosition, bool Pinned, float Radius, float Friction)
			{
				position = NewPosition;
				previousPosition = position;
				velocity = Vector3{ 0.0f };

				isPinned = Pinned;
				radius = Radius;
				friction = Friction;

			}
			Vector3 position = Vector3{0.0f};
			Vector3 previousPosition = Vector3{ 0.0f };
			Vector3 velocity = Vector3{ 0.0f };

			bool isPinned = false;
			float radius = 1.0f;
			float friction = 0.5f;

		};


		class Cloth
		{
		public:
			Cloth();
			Cloth(int n, float RangeBetweenPoints);

			void Simulate(float deltaTime);
			void UpdateMesh();

			SharedPtr<Mesh> clothMesh = nullptr;

			~Cloth();

		private:
		

			void CalculateNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount,
				unsigned int vLength, unsigned int normalOffset);

			int numberofSegments = 0;
			float rangeBetweenPoints = 0.0f;

			std::vector<SoftBodyParticle> particles;

			inline int IndexFrom2D(int x, int y);

			
		};


	}
}

#endif // !CLOTH_H

