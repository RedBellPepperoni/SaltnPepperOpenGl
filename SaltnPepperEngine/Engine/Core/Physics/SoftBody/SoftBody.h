#ifndef SOFTBODY_H
#define SOFTBODY_H
// Holy grail of windows funxtions
#include <Windows.h>	
#define WIN32_LEAN_AND_MEAN	   

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Components/SceneComponents.h"
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

	using Components::Ball;
	


	namespace Physics
	{
		class Hash
		{
		public:
			Hash(int newspacing, uint32_t maxnumObjects);
			~Hash() {};

			int HashCords(int xi, int yi, int zi);
			int IntCoords(const float& axisdata);

			int HashPosition(const Vector3& data);

			void Create(const std::vector<Vector3>& positions);
			void Query(const Vector3& position, const float& maxDistance);


		private:

			int spacing;
			size_t tableSize;
			std::vector<int> cellStart;
			std::vector<int> cellEntries;
			std::vector<int> queryIds;

			size_t querySize = 0;
			
		};

		struct TetMesh
		{
			std::string name;
			std::vector<float> Vertices;
			std::vector<uint32_t> TetIds;
			std::vector<uint32_t> TetEdgeIds;
			std::vector<uint32_t> TetSurfaceTriIds;
		};

		struct VisualMesh
		{
			std::vector<float> Vertices;
			std::vector<uint32_t> Indicie;
		};

		class SoftBody
		{
		public:

			SoftBody(SharedPtr<TetMesh>& tetmesh, const float& EdgeCompliance = 100.0f, const float& VolumeCompliance = 0.0f);
			~SoftBody(){};

			void OnInit(const Vector3& position);
			void UpdateMesh();

			SharedPtr<Mesh>& GetRenderMesh() { return renderMesh; }


		

			void PreSolve(const float& deltaTime, const Vector3& gravity = Vector3(0.0f, -10.0f, 0.0f));
			void Solve(const float& deltaTime);
			void PostSolve(const float& deltaTime);

			void SetPaused(const bool paused) { isPaused = paused; }

			void SetBallRef(SharedPtr<Ball> ballref)
			{
				ballRef = ballref;
			}

		private:

			//void SyncParticleTransform(const Transform& transform);

			void ComputeSkinningInfo(std::vector<Vector3>& visualVertices);
			float GetTetVolume(int index);


			void SolveEdges(const float& deltaTime);
			void SolveVolumes(const float& deltaTime);
		private:

			

			// Total number of particles to Spawn for this Mesh (Equal to the number of Vertices)
			size_t numParticles = 0;
			// The actual number of tetrahedral faces
			size_t numTets = 0;

			
			SharedPtr<Mesh> renderMesh = nullptr;
			SharedPtr<Ball> ballRef = nullptr;
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
			
			
			bool isPaused = false;
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
			void OnInit();

			void SetPaused(bool paused) { isPaused = paused; };
			const bool GetPaused() const { return isPaused; }
			void OnUpdate(const float& deltaTime);
			
			
			
		private:

			Vector3 gravity = Vector3(0.0f, -10.0f, 0.0f);
			
			std::vector<SharedPtr<SoftBody>> softBodyList;

			float fixedDeltaTime = 1.0f / 60.0f;
			int numSubsteps = 10;
			bool isPaused = true;
			float timestepCounter = 0.0f;
		};

		struct SoftBodyThreadInfo
		{	
			//The actual reference to the body, safer with smart pointer
			std::vector<SharedPtr<SoftBody>> softBodyHandles;

			SharedPtr<Ball> ballRef = nullptr;

			// the fixed deltatime to use for the simulation
			float fixedDeltatime = 0;

			// The gravitational force
			Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);

			//Thread run and alive info and sleep duration

			bool run = false;
			bool isAlive = false;
			DWORD sleepTime = 1;
			int numberofSubsetps = 12;

		};

		DWORD WINAPI UpdateThreadedSoftBody(LPVOID lpParameter);

		class ThreadedSolver
		{
		public:
			ThreadedSolver(const int maxThreadcount = 16);
			~ThreadedSolver();

			void OnInit();

			

			
			// call this function only Once
			void SetupSoftBodyThreads(SharedPtr<Ball> ballRef);

			void OnUpdate(const float deltaTime);

			void SetPaused(bool paused);
			const bool GetPaused() const { return isPaused; }
		private:


			int maximumThreadCount = 16;

			Vector3 gravity = Vector3(0.0f, -10.0f, 0.0f);
			float fixedDeltaTime = 1.0f / 60.0f;
			int numSubsteps = 10;
			bool isPaused = true;
			float timestepCounter = 0.0f;

			int bodyPerThread = 10;

			std::vector<SharedPtr<SoftBody>> softBodies;
			std::vector<SharedPtr<SoftBodyThreadInfo>> threadInfoList;
		};


	}
}


#endif // !SOFTBODY_H

