#include "Simulation.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Rendering/Material/Material.h"
#include "Engine/Utils/Maths/Random.h"



namespace SaltnPepperEngine
{
	using namespace Rendering;

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

		for (int y = 0; y < numPoints; y++)
		{
			float ty = y / (numPoints - 1.0f);
			for (int x = 0; x < numPoints; x++)
			{
				bool locked = y == 0 && x % 5 == 0;
				float tx = x / (numPoints - 1.0f);
				int i = IndexFrom2DCoord(x, y);
				Vector3 position = Vector3((tx - 0.5f) * sizeofCloth.x * 0.7f, (0.5f - ty) * sizeofCloth.y * 0.7f, 0.0f);

				Point newPoint;
				newPoint.position = position;
				newPoint.prevPosition = position;
				newPoint.pinned = locked;

				pointList.push_back(newPoint);


				Vertex vertexdata;

				vertexdata.position = position;
				vertexdata.texCoord = Vector2(((float)(x / (float)(numPoints - 1))), (float)(y / (float)(numPoints - 1)));
				vertexdata.normal = Vector3(0.0f, 0.0f, -1.0f);

				vertexList.push_back(vertexdata);
			}
		}

		float length = Length(pointList[0].position - pointList[1].position);

		for (int y = 0; y < numPoints; y++)
		{
			for (int x = 0; x < numPoints; x++)
			{
				if (x < numPoints - 1)
				{

					Stick newStick;
					newStick.pointOne = pointList[IndexFrom2DCoord(x, y)];
					newStick.pointTwo = pointList[IndexFrom2DCoord(x + 1, y)];

					stickList.push_back(newStick);
				}
				if (y < numPoints - 1)
				{
					Stick newStick;
					newStick.pointOne = pointList[IndexFrom2DCoord(x, y)];
					newStick.pointTwo = pointList[IndexFrom2DCoord(x, y + 1)];

					stickList.push_back(newStick);
				}
			}
		}

		CreateOrderArray();

		
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

	void Simulation::Cut(const Vector3& start, const Vector3& end)
	{
		for (int i = stickList.size() - 1; i >= 0; i--)
		{
			if (LineSegmentIntersect(start, end, stickList[i].pointOne.position, stickList[i].pointTwo.position))
			{
				stickList[i].cut = true;
			}
		}
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
		for(Point& p : pointList)
		{
			if (!p.pinned)
			{
				Vector3 positionBeforeUpdate = p.position;
				p.position += p.position - p.prevPosition;
				p.position += gravity * fixeddeltaTime * fixeddeltaTime;
				p.prevPosition = positionBeforeUpdate;
			}
		}

		for (int i = 0; i < solveIteration; i++)
		{
			for (int s = 0; s < stickList.size(); s++)
			{
				Stick stick = stickList[order[s]];
				if (stick.cut)
				{
					continue;
				}

				Vector3 stickCentre = (stick.pointOne.position + stick.pointTwo.position) / 2.0f;
				Vector3 stickDir = Normalize(stick.pointOne.position - stick.pointTwo.position);
				float length = Length(stick.pointOne.position - stick.pointTwo.position);

				if (length > stick.length || constraintStickMinLength)
				{
					if (!stick.pointOne.pinned)
					{
						stick.pointOne.position = stickCentre + stickDir * stick.length / 2.0f;
					}
					if (!stick.pointTwo.pinned)
					{
						stick.pointTwo.position = stickCentre - stickDir * stick.length / 2.0f;
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

		ShuffleArray(order);


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