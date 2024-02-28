#include "VerletCloth.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Rendering/Material/Material.h"

namespace SaltnPepperEngine
{
	using namespace	Rendering;

	namespace Physics
	{
	
		VerletCloth::VerletCloth(int NumNodesPerAxis, float RangeBetweenPoints)
		{
			numberofSegments = NumNodesPerAxis;
			rangeBetweenPoints = RangeBetweenPoints;


			std::vector<Vertex> vertexList;
			std::vector<uint32_t> indexList;

			for (int i = 0; i < numberofSegments; i++)
			{
				for (int j = 0; j < numberofSegments; j++)
				{
					// Pin the side particles
					bool pinned = (i == 0);
					Vector3 position = Vector3(0.0f, j * rangeBetweenPoints, i * rangeBetweenPoints);
					VerletParticle currentPoint = VerletParticle(position, pinned, 0.125f);
					particles.push_back(currentPoint);

					Vertex vertex;

					vertex.position = position;
					vertex.texCoord = Vector2(((float)(j / (float)(numberofSegments - 1))), (float)(i / (float)(numberofSegments - 1)));
					vertex.normal = Vector3(0.0f, 0.0f, 1.0f);

					vertexList.push_back(vertex);

				}
			}

			for (int i = 0; i < numberofSegments - 1; i++) {
				for (int j = 0; j < numberofSegments - 1; j++) {
					indexList.push_back(i * numberofSegments + j);
					indexList.push_back(numberofSegments * (i + 1) + j + 1);
					indexList.push_back(i * numberofSegments + j + 1);

					indexList.push_back(i * numberofSegments + j);
					indexList.push_back(numberofSegments * (i + 1) + j);
					indexList.push_back(numberofSegments * (i + 1) + j + 1);
				}
			}



			clothMesh = MakeShared<Mesh>(vertexList, indexList);
			
			UpdateRenderMesh();
		}

		void VerletCloth::OnInit(const Transform& Transform)
		{
		}

		void VerletCloth::OnUpdate(const float& deltaTime, const Transform& clothtransform)
		{
			for (int i = 0; i < substeps; ++i)
			{
				Simulate(deltaTime / (float)substeps, clothtransform);
				//UpdateMesh();
			}
			// Only Updating the Render mesh once per update
			UpdateRenderMesh();
		}

		SharedPtr<Material>& VerletCloth::GetMaterial()
		{
			return clothMesh->GetMaterial();
		}
		SharedPtr<Mesh>& VerletCloth::GetMesh()
		{
			return clothMesh;
		}
		void VerletCloth::Simulate(const float& deltaTime, const Transform& clothtransform)
		{
			timeStepCounter += deltaTime;
			if (timeStepCounter < fixedDeltaTime)
			{
				return;
			}

			timeStepCounter = 0.0f;

			for (int i = 0; i < particles.size(); i++)
			{
				if (!particles[i].isPinned)
				{

					Vector3 prevPos = particles[i].position;
					Vector3 displacement = particles[i].position - particles[i].previousPosition;
					particles[i].velocity = displacement / fixedDeltaTime;
					particles[i].position += displacement;
					particles[i].position += Vector3(0.0f, -10.0f, 0.0f) * fixedDeltaTime * fixedDeltaTime;
					particles[i].position += force * fixedDeltaTime * fixedDeltaTime;
					

					float speed = glm::sqrt(Length(particles[i].velocity));
					particles[i].position -= displacement * particles[i].friction * glm::pow(speed, 2.0f) * fixedDeltaTime;
					particles[i].previousPosition = prevPos;
				}
			}

			for (int i = 0; i < numberofSegments; i++)
			{
				for (int j = 0; j < numberofSegments; j++)
				{
					int index0 = IndexFrom2D(j, i); 
					int index1 = IndexFrom2D(j + 1, i); 
					int index2 = IndexFrom2D(j, i + 1); 


					Vector3 stickCenter;
					Vector3 stickDir;
					float length;

					if (j < numberofSegments - 1)
					{
						stickCenter = (particles[index0].position + particles[index1].position) * 0.5f;
						stickDir = Normalize(particles[index0].position - particles[index1].position);
						length = Length(particles[index0].position - particles[index1].position);

						if (length > rangeBetweenPoints)
						{
							if (!particles[index0].isPinned)
								particles[index0].position = stickCenter + stickDir * 0.5f * rangeBetweenPoints;
							if (!particles[index1].isPinned)
								particles[index1].position = stickCenter - stickDir * 0.5f * rangeBetweenPoints;
						}
					}
					if (i < numberofSegments - 1)
					{
						stickCenter = (particles[index0].position + particles[index2].position) * 0.5f;
						stickDir = Normalize(particles[index0].position - particles[index2].position);
						length = Length(particles[index0].position - particles[index2].position);

						if (length > rangeBetweenPoints)
						{
							if (!particles[index0].isPinned)
							{
								particles[index0].position = stickCenter + stickDir * 0.5f * rangeBetweenPoints;
							}

							if (!particles[index2].isPinned)
							{
								particles[index2].position = stickCenter - stickDir * 0.5f * rangeBetweenPoints;
							}
						}
					}
				}
			}
		}

		void VerletCloth::UpdateRenderMesh()
		{
			std::vector<Vertex> vertexList;
			std::vector<uint32_t> indexList;

			for (int i = 0; i < numberofSegments; i++)
			{
				for (int j = 0; j < numberofSegments; j++)
				{

					Vertex newVert;

					newVert.position = particles[IndexFrom2D(j, i)].position; 
					newVert.texCoord = Vector2(((float)(j / (float)(numberofSegments - 1))), (float)(i / (float)(numberofSegments - 1))); 
					newVert.normal = Vector3(0.0f, 0.0f, 1.0f);
					vertexList.push_back(newVert);
				}
			}



			
			Mesh::RecalculateNormals(vertexList, clothMesh->GetIndexData()); 
			clothMesh->SetVertexData(vertexList); 
			clothMesh->GetVBO()->SetSubData(0, sizeof(Vertex) * clothMesh->GetVertexCount(), vertexList.data()); 
			clothMesh->GetVBO()->UnBind(); 
		}

		inline int VerletCloth::IndexFrom2D(int x, int y)
		{
			return y * numberofSegments + x;
		}
	}
}