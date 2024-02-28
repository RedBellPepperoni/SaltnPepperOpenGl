#include "Cloth.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"

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
					// Pin the side particles
					bool pinned = (j == 0);
					Vector3 position = Vector3(j * rangeBetweenPoints, 0.0f, i * rangeBetweenPoints);
					SoftBodyParticle currentPoint = SoftBodyParticle(position,pinned,0.125f,0.75f); 
					particles.push_back(currentPoint);

					Vertex vertex;

					vertex.position = position;
					vertex.texCoord = Vector2(((float)(j / (float)(n - 1))), (float)(i / (float)(n - 1)));
					vertex.normal = Vector3(0.0f, 0.0f, 1.0f);

					vertexList.push_back(vertex);
					
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
			particles;

			UpdateMesh();
		}


	

		void Cloth::OnInit(Transform* Transform, SharedPtr<SphereCollider>& collider)
		{
			sphereTransform = Transform;
			sphere = collider;
		}

		void Cloth::OnUpdate(float deltaTime, const Transform& ballTransform, const Transform& clothTransform)
		{
			for (int i = 0; i < substeps; ++i)
			{
				Simulate(deltaTime/(float)substeps, ballTransform,clothTransform);
				//UpdateMesh();
			}
			// Only Updating the Render mesh once per update
			UpdateMesh();

		}

		void Cloth::Simulate(float deltaTime, const Transform& ballTransform, const Transform& clothTransform)
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
					particles[i].position += Vector3(0.0f, -1.0f, 0.0f) * 9.81f * fixedDeltaTime * fixedDeltaTime;

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

					SphereClothCollision(particles[index0], ballTransform,clothTransform);

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
		void Cloth::UpdateMesh()
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


			
			//clothMesh->RecalculateNormals(vertexList,clothMesh->GetIndexData());
			Mesh::RecalculateNormals(vertexList,clothMesh->GetIndexData());
			clothMesh->SetVertexData(vertexList);
			clothMesh->GetVBO()->SetSubData(0, sizeof(Vertex) * clothMesh->GetVertexCount(), vertexList.data());
			clothMesh->GetVBO()->UnBind();

		}
		Cloth::~Cloth()
		{
		}


		void Cloth::SphereClothCollision(SoftBodyParticle& particle,const Transform& ballTransform, const Transform& clothTransform )
		{
			Vector3 spherePosition = ballTransform.GetPosition();

			Vector3 diff = (particle.position + clothTransform.GetPosition()) - spherePosition;
			float radiusSum = particle.radius + sphere->GetRadius();

			// Basic Intesecting logic
			bool isIntersect = Abs(Dot(diff, diff)) <= radiusSum * radiusSum;

			if (isIntersect) 
			{
				Vector3 normal = Normalize((particle.position + clothTransform.GetPosition()) - spherePosition);
				float distance = Abs(Length(spherePosition - (particle.position + clothTransform.GetPosition())));
				float depth = radiusSum - distance;

				particle.position += normal * depth;
			}
		}


		int Cloth::IndexFrom2D(int x, int y)
		{
			return y * numberofSegments + x;
		}
	}
}