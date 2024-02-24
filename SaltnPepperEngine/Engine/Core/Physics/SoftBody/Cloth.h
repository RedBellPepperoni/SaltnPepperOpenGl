#ifndef CLOTH_H
#define CLOTH_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{
	namespace Components
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

	namespace Physics
	{
		class SphereCollider;

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

			void OnInit(Transform* Transform, SharedPtr<SphereCollider>& collider);
			void OnUpdate(float deltaTime,const Transform& ballTransform,const Transform& clothTransform);

			void Simulate(float deltaTime,const Transform& ballTransform, const Transform& clothTransform);
			void UpdateMesh();

			SharedPtr<Mesh> clothMesh = nullptr;

			~Cloth();

		private:
		


			void SphereClothCollision(SoftBodyParticle& particle,  const Transform& ballTransform, const Transform& clothTransform);

			int numberofSegments = 0;
			float rangeBetweenPoints = 0.0f;

			std::vector<SoftBodyParticle> particles;
			Transform* sphereTransform = nullptr;
			SharedPtr<SphereCollider> sphere = nullptr;

			inline int IndexFrom2D(int x, int y);

			const float substeps = 10;
		};


		struct ClothComponent
		{
			ClothComponent(uint32_t numberSegments = 8) { clothHandle = MakeShared<Cloth>(numberSegments, 0.5f); }

			SharedPtr<Cloth> clothHandle;
		};


	}
}

#endif // !CLOTH_H
