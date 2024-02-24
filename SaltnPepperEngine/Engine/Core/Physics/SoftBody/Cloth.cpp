#include "Cloth.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;


	namespace Physics
	{
		Cloth::Cloth()
		{
		}

		Cloth::Cloth(int n, float RangeBetweenPoints)
		{
			numberofSegments = n;
			rangeBetweenPoints = RangeBetweenPoints;


			std::vector<Vertex> vertexList;
			std::vector<uint32_t> indexList;

			for (int i = 0; i < n; i++) 
			{
				for (int j = 0; j < n; j++) 
				{
					// Pin the top most particles
					bool pinned = (i == 0);
					Vector3 position = glm::vec3(j * rangeBetweenPoints, 0.0f, i * rangeBetweenPoints);
					SoftBodyParticle currentPoint = SoftBodyParticle(position,pinned,0.125f,0.75f); 
					particles.push_back(currentPoint);

					Vertex vertex;

					vertex.position = position;
					vertex.texCoord = Vector2(((float)(j / (float)(n - 1))), (float)(i / (float)(n - 1)));
					vertex.normal = Vector3(0.0f, 0.0f, 1.0f);
					
				}
			}

			for (int i = 0; i < n - 1; i++) {
				for (int j = 0; j < n - 1; j++) {
					indexList.push_back(i * n + j);
					indexList.push_back(n * (i + 1) + j + 1);
					indexList.push_back(i * n + j + 1);

					indexList.push_back(i * n + j);
					indexList.push_back(n * (i + 1) + j);
					indexList.push_back(n * (i + 1) + j + 1);
				}
			}

			

			clothMesh = MakeShared<Mesh>(vertexList,indexList);
		}


		void Cloth::Simulate(float deltaTime)
		{
		}
		void Cloth::UpdateMesh()
		{
		}
		Cloth::~Cloth()
		{
		}
		void Cloth::CalculateNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
		{
		}
		int Cloth::IndexFrom2D(int x, int y)
		{
			return y * numberofSegments + x;
		}
	}
}