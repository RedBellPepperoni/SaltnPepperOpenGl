#ifndef SIMULATION_H
#define SIMULATION_H
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

	namespace Verlet {

		struct Point
		{
			Vector3 position;
			Vector3 prevPosition;
			bool pinned;
		};

		struct Stick
		{
			Point* pointOne;
			Point* pointTwo;

			float length = 0.5f;
			bool cut = false;
		};

		class Simulation
		{

		public:


			void OnInit(const int& clothSegments, const Vector2& clothSize);

			void Cut(const Vector3& start, const Vector3& end);
			void Cut(Vector2Int node);
			void OnUpdate(const float& deltaTime);

			SharedPtr<Mesh> GetMesh();
			SharedPtr<Material> GetMaterial();
		private:

			void Simulate(const float& fixeddeltaTime);
			void UpdateRenderMesh();

			void CreateOrderArray();
			int IndexFrom2DCoord(const int& x, const int& y);

			static bool LineSegmentIntersect(const Vector3& a1, const Vector3& a2, const Vector3& b1, const Vector3& b2);

			template< typename T>
			static void ShuffleArray(std::vector<T>& array);

			// Variables
		public:


			int numPoints = 8;
			Vector2 sizeofCloth = Vector2(2.0f, 1.5f);
			Vector3 cutPosOld = Vector3(0.0f);

			float pointRadius = 0.0f;
			int stickStartindex = 0;

			bool isSimulating = false;
			bool constraintStickMinLength = false;
			bool autoStickMode = false;
			int solveIteration = 5;

			Vector3 gravity = Vector3(0.0f, -10.0f, 0.0f);
			Vector3 wind = Vector3(0.0f);

			std::vector<int> cutPoints;

		protected:

			float timeCounter = 0.0f;

			float fixedDeltatime = 1.0f / 60.0f;



			std::vector<Point> pointList;
			std::vector<Stick> stickList;
			std::vector<int> order;

			SharedPtr<Mesh> renderMesh = nullptr;
		};


		struct VerletClothComponent
		{
			VerletClothComponent(const int& numSegments = 16, const Vector2& clothSize = Vector2(5.0f, 3.0f))
			{
				clothsim = MakeShared<Simulation>();
				
			}

			SharedPtr<Simulation> clothsim = nullptr;
		};
	}

	using namespace Verlet;

}

#endif // !SIMULATION_H