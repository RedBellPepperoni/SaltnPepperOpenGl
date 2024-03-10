#ifndef CLOTH_H
#define CLOTH_H
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Components/SceneComponents.h"
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
	using Components::Ball;

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

			void OnInit(Transform& clothTransform);
			void SetBallRef(SharedPtr<Ball> ball);
			void OnUpdate(float deltaTime);

			void Simulate(float deltaTime);
			void UpdateMesh();

			SharedPtr<Mesh> clothMesh = nullptr;

			~Cloth();

			const bool GetPaused() const { return paused; }
			void SetPaused(bool value) { paused = value; }

		private:
		
			Vector3 InitPosition = Vector3(0.0f);
			SharedPtr<Ball> ballRef = nullptr;
			void SphereClothCollision(SoftBodyParticle& particle);

			int numberofSegments = 0;
			float rangeBetweenPoints = 0.0f;

			std::vector<SoftBodyParticle> particles;
			Transform* sphereTransform = nullptr;
			SharedPtr<SphereCollider> sphere = nullptr;

			inline int IndexFrom2D(int x, int y);

			const float substeps = 12;

			static constexpr float fixedDeltaTime = 1.0f / 60.0f;
			float timeStepCounter = 0.0f;

			bool paused = false;
		};


		struct ClothComponent
		{
			ClothComponent(uint32_t numberSegments = 8) { clothHandle = MakeShared<Cloth>(numberSegments, 0.5f); }

			SharedPtr<Cloth> clothHandle = nullptr;
		};


		struct ClothThreadInfo
		{
			//The actual reference to the body, safer with smart pointer
			std::vector<SharedPtr<Cloth>> ClothHandles;

			// the fixed deltatime to use for the simulation
			float fixedDeltatime = 1 / 50.0f;

			// The gravitational force
			Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);

			//Thread run and alive info and sleep duration

			bool run = false;
			bool isAlive = false;
			DWORD sleepTime = 1;
			int numberofSubsetps = 12;

		};

		DWORD WINAPI UpdateThreadedCloth(LPVOID lpParameter);

		class ThreadedClothSolver
		{
		public:
			ThreadedClothSolver(const int maxThreadcount = 4);
			~ThreadedClothSolver();

			void OnInit();




			// call this function only Once
			void SetupClothThreads(SharedPtr<Ball> ballRef);

			void OnUpdate(const float deltaTime);

			void SetPaused(bool paused);
			const bool GetPaused() const { return isPaused; }
		private:


			int maximumThreadCount = 16;

			Vector3 gravity = Vector3(0.0f, -10.0f, 0.0f);
			float fixedDeltaTime = 1.0f / 60.0f;
			int numSubsteps = 16;
			bool isPaused = true;
			float timestepCounter = 0.0f;

			int bodyPerThread = 10;

			std::vector<SharedPtr<Cloth>> clothRefs;
			std::vector<SharedPtr<ClothThreadInfo>> threadInfoList;
		};

	}
}

#endif // !CLOTH_H

