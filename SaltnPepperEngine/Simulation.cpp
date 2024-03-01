#include "Simulation.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/Buffers/IndexBuffer.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/Maths/Random.h"



namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Verlet 
	{

		template<typename T>
		inline void Simulation::ShuffleArray(std::vector<T>& array)
		{
			int elementsRemainingToShuffle = array.size();
			int randomIndex = 0;

			while (elementsRemainingToShuffle > 1)
			{

				// Choose a random element from array
				randomIndex = Random32::Range.GetRandom(0, elementsRemainingToShuffle);
				T chosenElement = array[randomIndex];

				// Swap the randomly chosen element with the last unshuffled element in the array
				elementsRemainingToShuffle--;
				array[randomIndex] = array[elementsRemainingToShuffle];
				array[elementsRemainingToShuffle] = chosenElement;
			}

		}


		void Simulation::OnInit(const int& clothSegments, const Vector2& clothSize)
		{
			numPoints = clothSegments;
			sizeofCloth = clothSize;

			std::vector<Vertex> vertexList;
			std::vector<uint32_t> indexList;

			Vertex vertexdata;
			Point newPoint;

			for (int y = 0; y < numPoints; y++)
			{
				float ty = y / (numPoints - 1.0f);
				for (int x = 0; x < numPoints; x++)
				{
					//bool locked = y == 0 && x % 5 == 0;
					bool locked = x == 0;
					float tx = x / (numPoints - 1.0f);
					int i = IndexFrom2DCoord(x, y);
					Vector3 position = Vector3((tx - 0.5f) * sizeofCloth.x * 0.7f, (0.5f - ty) * sizeofCloth.y * 0.7f, 0.0f);

					
					newPoint.position = position;
					newPoint.prevPosition = position;
					newPoint.pinned = locked;

					pointList.push_back(newPoint);


					vertexdata.position = position;
					vertexdata.texCoord = Vector2(((float)(x / (float)(numPoints - 1))), (float)(y / (float)(numPoints - 1)));
					vertexdata.normal = Vector3(0.0f, 0.0f, -1.0f);

					vertexList.push_back(vertexdata);
				}
			}

			//float length = Length(pointList[0].position - pointList[1].position);

			for (int y = 0; y < numPoints; y++)
			{
				for (int x = 0; x < numPoints; x++)
				{
					if (x < numPoints - 1)
					{

						Stick newStick;
						newStick.pointOne = &pointList[IndexFrom2DCoord(x, y)];
						newStick.pointTwo = &pointList[IndexFrom2DCoord(x + 1, y)];
						newStick.length = Distance(newStick.pointOne->position, newStick.pointTwo->position);
						//newStick.length = 1.0f;
						stickList.push_back(newStick);
					}
					if (y < numPoints - 1)
					{
						Stick newStick;
						newStick.pointOne = &pointList[IndexFrom2DCoord(x, y)];
						newStick.pointTwo = &pointList[IndexFrom2DCoord(x, y + 1)];
						newStick.length = Distance(newStick.pointOne->position, newStick.pointTwo->position);
						//newStick.length = 1.0f;
						stickList.push_back(newStick);
					}
				}
			}

			//CreateOrderArray();


			for (int x = 0; x < numPoints - 1; x++) {
				for (int y = 0; y < numPoints - 1; y++) {
					indexList.push_back(x * numPoints + y);
					indexList.push_back(numPoints * (x + 1) + y + 1);
					indexList.push_back(x * numPoints + y + 1);

					indexList.push_back(x * numPoints + y);
					indexList.push_back(numPoints * (x + 1) + y);
					indexList.push_back(numPoints * (x + 1) + y + 1);
				}
			}

			renderMesh = MakeShared<Mesh>(vertexList, indexList);

			UpdateRenderMesh();
		}

		void Simulation::Cut()
		{
			//
			// for()
		}

		void Simulation::Cut(Vector2Int node)
		{
			int index = IndexFrom2DCoord(node.x, node.y);

			if (index > pointList.size() || index < 0)
			{
				return;
			}


			Point* point = &pointList[index];
			
			cutPoints.push_back(index);
			

			std::vector<Stick*> tempsticks;

			for (int i = 0; i < stickList.size(); i++)
			{
				if (point == stickList[i].pointOne || point == stickList[i].pointTwo)
				{
					Stick* stick = &stickList[i];
					stick->cut = true;
					tempsticks.push_back(stick);
				}
			}

			tempsticks;


			

			std::vector<uint32_t> indexList;

			indexList.clear();

			for (int i = 0; i < numPoints - 1; i++)
			{

				for (int j = 0; j < numPoints - 1; j++)
				{

					int index0 = j * numPoints + i;
					int index1 = numPoints * (j + 1) + i + 1;
					int index2 = j * numPoints + i + 1;
					int index3 = numPoints * (j + 1) + i;

					indexList.push_back(index0); //0
					indexList.push_back(index1); //n + 1
					indexList.push_back(index2); //1



					indexList.push_back(index0);
					indexList.push_back(index3);
					indexList.push_back(index1);

					for (int& node : cutPoints)
					{
						

						if (index0 == node || index1 == node || index2 == node || index3 == node)
						{
							indexList.pop_back();
							indexList.pop_back();
							indexList.pop_back();

							indexList.pop_back();
							indexList.pop_back();
							indexList.pop_back();
						}

	
					}


					
				}
			}

			



			


			renderMesh->GetIBO()->Bind();
			renderMesh->GetIBO()->SetSubData(0,indexList.size() * sizeof(uint32_t),indexList.data());
			renderMesh->GetIBO()->UnBind();

			renderMesh->SetIndexData(indexList);

			UpdateRenderMesh();
		}

		void Simulation::OnUpdate(const float& deltaTime)
		{
			timeCounter += deltaTime;

			if (timeCounter < fixedDeltatime)
			{
				return;
			}

			timeCounter = 0.0f;

			Simulate(fixedDeltatime);
			UpdateRenderMesh();
		}

		SharedPtr<Mesh> Simulation::GetMesh()
		{
			return renderMesh;
		}

		SharedPtr<Material> Simulation::GetMaterial()
		{
			return renderMesh->GetMaterial();
		}

		void Simulation::Simulate(const float& fixeddeltaTime)
		{
			for (int index = 0; index < pointList.size(); index ++)
			{
				if (!pointList[index].pinned)
				{
					Vector3 positionBeforeUpdate = pointList[index].position;
					pointList[index].position += pointList[index].position - pointList[index].prevPosition;
					pointList[index].position += gravity * fixeddeltaTime * fixeddeltaTime;
					pointList[index].prevPosition = positionBeforeUpdate;
				}
			}

			//for (int i = 0; i < solveIteration; i++)
			for (int i = 0; i < solveIteration; i++)
			{
				for (int s = 0; s < stickList.size(); s++)
				{
					//Stick& stick = stickList[order[s]];
					Stick& stick = stickList[s];
					if (stick.cut)
					{
						continue;
					}

					Vector3 stickCentre = (stick.pointOne->position + stick.pointTwo->position) / 2.0f;
					Vector3 stickDir = Normalize(stick.pointOne->position - stick.pointTwo->position);
					float length = Length(stick.pointOne->position - stick.pointTwo->position);

					if (length > stick.length)
					{
						if (!stick.pointOne->pinned)
						{
							stick.pointOne->position = stickCentre + stickDir * stick.length / 2.0f;
						}
						if (!stick.pointTwo->pinned)
						{
							stick.pointTwo->position = stickCentre - stickDir * stick.length / 2.0f;
						}
					}

				}
			}
		}

		void Simulation::UpdateRenderMesh()
		{
			std::vector<Vertex> vertexList;
			std::vector<uint32_t> indexList;

			for (int i = 0; i < numPoints; i++)
			{
				for (int j = 0; j < numPoints; j++)
				{

					Vertex newVert;

					newVert.position = pointList[IndexFrom2DCoord(j, i)].position;
					newVert.texCoord = Vector2(((float)(j / (float)(numPoints - 1))), (float)(i / (float)(numPoints - 1)));
					newVert.normal = Vector3(0.0f, 0.0f, 1.0f);
					vertexList.push_back(newVert);
				}
			}




			Mesh::RecalculateNormals(vertexList, renderMesh->GetIndexData());
			renderMesh->SetVertexData(vertexList);
			renderMesh->GetVBO()->SetSubData(0, sizeof(Vertex) * renderMesh->GetVertexCount(), vertexList.data());
			renderMesh->GetVBO()->UnBind();
		}

		void Simulation::CreateOrderArray()
		{
			// Initialize teh order list
			order = std::vector<int>(stickList.size(), 0);

			for (int index = 0; index < order.size(); index++)
			{
				order[index] = index;
			}

			//ShuffleArray(order);


		}
		int Simulation::IndexFrom2DCoord(const int& x, const int& y)
		{
			return y * numPoints + x;
		}
		bool Simulation::LineSegmentIntersect(const Vector3& a1, const Vector3& a2, const Vector3& b1, const Vector3& b2)
		{
			return false;
		}
	}
}