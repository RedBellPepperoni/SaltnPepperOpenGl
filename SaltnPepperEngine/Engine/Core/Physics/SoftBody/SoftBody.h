#ifndef SOFTBODY_H
#define SOFTBODY_H
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <vector>
#include <string>

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class Mesh;
	}

	using Rendering::Mesh;

	namespace Components
	{
		class Transform;
	}

	using Components::Transform;

	namespace Physics
	{

		namespace SoftBodyMath
		{
			
		}

		struct TetMesh
		{
			std::string name;
			std::vector<float> Vertices;
			std::vector<uint32_t> TetIds;
			std::vector<uint32_t> TetEdgeIds;
			std::vector<uint32_t> TetSurfaceTriIds;
		};

		class SoftBody
		{
		public:

			SoftBody(SharedPtr<TetMesh>& tetmesh, const float& EdgeCompliance = 0.1f, const float& VolumeCompliance = 0.13f);
			~SoftBody(){};

			void OnInit(const Vector3& position);
			void UpdateMesh();

			SharedPtr<Mesh>& GetRenderMesh() { return renderMesh; }


			void PreSolve(const float& deltaTime, const Vector3& gravity = Vector3(0.0f, -9.81f, 0.0f));
			void Solve(const float& deltaTime);
			void PostSolve(const float& deltaTime);

		private:

			//void SyncParticleTransform(const Transform& transform);

			float GetTetVolume(int index);


			void SolveEdges(const float& deltaTime);
			void SolveVolumes(const float& deltaTime);
		private:

			

			// Total number of particles to Spawn for this Mesh (Equal to the number of Vertices)
			size_t numParticles = 0;
			// The actual number of tetrahedral faces
			size_t numTets = 0;

			
			SharedPtr<Mesh> renderMesh = nullptr;

			std::vector<Vector3> vertPositions{};
			std::vector<Vector3> vertPreviousPositions{};
			std::vector<Vector3> particleVelocityList{};

			std::vector<uint32_t> edgeIdList{};
			std::vector<uint32_t> tetIdList{};

			std::vector<float> inverseMassList{};
			std::vector<float> edgeLengthList{};


			std::vector<float> restVolume{};

			float edgeCompliance = 0.0f;
			float volumeCompliance = 0.0f;

			static constexpr uint8_t volumeIdOrder[4][3] = { {1,3,2}, {0,2,3},{0,3,1},{0,1,2} };
			
			
		};

		// ECS Component
		class SoftBodyComponent
		{
		public:

			SoftBodyComponent(SharedPtr<TetMesh>& tetmesh)
			{
				softBodyhandle = MakeShared<SoftBody>(tetmesh);
			}

			SharedPtr<SoftBody> softBodyhandle = nullptr;
		};

		class Solver
		{
		public:
			Solver();
			~Solver();

			void OnUpdate(const float& deltaTime);
			
		private:

			Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);
			float fixedDeltaTime = 1.0f / 60.0f;
			int numSubsteps = 10;

			float timestepCounter = 0.0f;
		};


	}
}


#endif // !SOFTBODY_H

