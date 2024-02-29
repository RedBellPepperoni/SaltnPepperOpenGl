#ifndef VERLETCLOTH_H
#define VERLETCLOTH_H

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
		class Material;
		struct Vertex;
	}



	using Rendering::Mesh;
	using Rendering::Material;
	using Components::Transform;

	namespace Physics
	{
		struct VerletParticle
		{
			// Defauolt C'tor and D'tor
			VerletParticle() = default;
			~VerletParticle() = default;

			// Custom C'tor
			VerletParticle(const Vector3& NewPostiion, const bool& Pinned, const float Radius)
			{
				position = NewPostiion;
				previousPosition = position;
				velocity = Vector3{ 0.0f };

				isPinned = Pinned;
				radius = Radius;
				friction = 0.5f;
			}



			// POsitional Data for the nodes
			Vector3 position = Vector3{ 0.0f };
			Vector3 previousPosition = Vector3{ 0.0f };
			Vector3 velocity = Vector3{ 0.0f };

			// Extra node details
			bool isPinned = false;
			float radius = 1.0f;
			float friction = 0.89f;
		};


		class VerletCloth
		{
		public:

			// C'tors and D'tors
			VerletCloth() = default;
			~VerletCloth() = default;
			VerletCloth(int NumNodesPerAxis, float RangeBetweenPoints);

			// Initialization and Update
			void OnInit(const Transform& Transform);
			void OnUpdate(const float& deltaTime, const Transform& clothtransform);

			// Getters for Rendering stuff
			SharedPtr<Material>& GetMaterial();
			SharedPtr<Mesh>& GetMesh();

			void RemoveNode(const Vector2Int& node);

			void SetForce(const Vector3& NewForce) { force = NewForce; }

		private:

			// The internal fucntions for simulation
			void Simulate(const float& deltaTime, const Transform& clothtransform);
			void UpdateRenderMesh();
			inline int IndexFrom2D(int x, int y);


		private:

			// Reference to the Render mesh
			SharedPtr<Mesh> clothMesh = nullptr;

			// The number of segment for each axis(x and y) clothis always square
			int numberofSegments = 0;
			// The distance between the points (the stick length)
			float rangeBetweenPoints = 0.0f;

			// The list of all the verlet nodes/particles
			std::vector<VerletParticle> particles;
			
			// The substeps the physics engine will perform each fixed update
			const float substeps = 16;

			// Fixed update rate and counter
			static constexpr float fixedDeltaTime = 1.0f / 60.0f;
			float timeStepCounter = 0.0f;

			Vector3 force = Vector3(0.0f);
			
			std::vector<int> cutPoints;
		};

		struct ClothComponent
		{
			ClothComponent(uint32_t numberSegments = 8) { clothHandle = MakeShared<VerletCloth>(numberSegments, 0.5f); }

			SharedPtr<VerletCloth> clothHandle;
		};

	}
	
	
}

#endif // !VERLETCLOTH_H

