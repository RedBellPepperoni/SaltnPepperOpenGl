#include "Cloth.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/Physics/Collision/Colliders/SphereCollider.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Scene/Scene.h"

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
		}


	

		void Cloth::OnInit(Transform& clothTransform)
		{
			
			InitPosition = clothTransform.GetPosition();
			LOG_CRITICAL("Material : {0} :[{1}]", clothMesh->GetMaterial()->name, (uint64_t)clothMesh->GetMaterial().get());

		}

		void Cloth::SetBallRef(SharedPtr<Ball> ball)
		{
			ballRef = ball;
		}

		void Cloth::OnUpdate(float deltaTime)
		{
			//for (int i = 0; i < substeps; ++i)
			//{
			//	Simulate(deltaTime/(float)substeps);
			//	//UpdateMesh();
			//}
			// Only Updating the Render mesh once per update

			if (paused) { return; }

			UpdateMesh();

		}

		void Cloth::Simulate(float deltaTime)
		{
			if (paused) { return; }

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

					//SphereClothCollision(particles[index0]);

					SoftBodyParticle& particle = particles[index0];

					Vector3 spherePosition = ballRef->position;

					Vector3 diff = (particle.position + InitPosition) - spherePosition;
					float radiusSum = particle.radius + 1.0f;

					// Basic Intesecting logic
					bool isIntersect = Abs(Dot(diff, diff)) <= radiusSum * radiusSum;

					if (isIntersect)
					{
						Vector3 normal = Normalize((particle.position + InitPosition) - spherePosition);
						float distance = Abs(Length(spherePosition - (particle.position + InitPosition)));
						float depth = radiusSum - distance;

						particle.position += normal * depth;
					}

					/*if (particle.position.y < 0.0f)
					{
						particle.position.y = 0.0f;
					}*/

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


		void Cloth::SphereClothCollision(SoftBodyParticle& particle)
		{
			Vector3 spherePosition = ballRef->position;

			Vector3 diff = (particle.position + InitPosition) - spherePosition;
			float radiusSum = particle.radius + sphere->GetRadius();

			// Basic Intesecting logic
			bool isIntersect = Abs(Dot(diff, diff)) <= radiusSum * radiusSum;

			if (isIntersect) 
			{
				Vector3 normal = Normalize((particle.position + InitPosition) - spherePosition);
				float distance = Abs(Length(spherePosition - (particle.position + InitPosition)));
				float depth = radiusSum - distance;

				particle.position += normal * depth;
			}
		}


		int Cloth::IndexFrom2D(int x, int y)
		{
			return y * numberofSegments + x;
		}


		DWORD __stdcall UpdateThreadedCloth(LPVOID lpParameter)
		{
			ClothThreadInfo* threadInfo = (ClothThreadInfo*)lpParameter;



			double lastFrameTime = glfwGetTime();
			double counter = 0.0f;

			while (threadInfo->isAlive)
			{
				if (threadInfo->run)
				{
					// Adjust sleep time based on actual framerate
					//float delta = Time::DeltaTime();

					double currentTime = glfwGetTime();
					float deltaTime = (float)(currentTime - lastFrameTime);
					lastFrameTime = currentTime;

					counter += deltaTime;

					if (counter > threadInfo->fixedDeltatime)
					{
						counter = 0.0f;

						float substepDelta = threadInfo->fixedDeltatime / threadInfo->numberofSubsetps;

						for (int i = 0; i < threadInfo->numberofSubsetps; i++)
						{
							for (SharedPtr<Cloth>& body : threadInfo->ClothHandles)
							{	
								body->Simulate(substepDelta);
							}

						}


					}


					//Sleep(threadInfo->sleepTime);
					Sleep(0);


				}
			}

			return 0;
		}

		ThreadedClothSolver::ThreadedClothSolver(const int maxThreadcount)
		{
			maximumThreadCount = maxThreadcount;
		}

		ThreadedClothSolver::~ThreadedClothSolver()
		{

		}

		void ThreadedClothSolver::OnInit()
		{
			ComponentView clothView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<ClothComponent>();

			for (Entity clothComp : clothView)
			{
				// Get the softbody handle from the compoenet
				SharedPtr<Cloth> bodyref = clothComp.GetComponent<ClothComponent>().clothHandle;
				// Store the reference for faster access
				clothRefs.push_back(bodyref);

				bodyref->SetPaused(isPaused);
			}


		}

		void ThreadedClothSolver::SetupClothThreads(SharedPtr<Ball> ballRef)
		{
			int clothCount = (int)clothRefs.size();

			//int bodyPerThread = softBodyCount / maximumThreadCount;

			if (clothCount > maximumThreadCount * bodyPerThread)
			{
				int difference = (maximumThreadCount * bodyPerThread) - clothCount;

				LOG_CRITICAL("Large Number of SoftBodies : deleting {0} out of {1}", difference, clothCount);


				clothRefs.resize(maximumThreadCount * bodyPerThread);
			}




			int newbodyCount = (int)(clothRefs).size();

			int threadstouse = newbodyCount % bodyPerThread == 0 ? newbodyCount / bodyPerThread : newbodyCount / bodyPerThread + 1;
			int bodyCounter = 0;

			for (int i = 0; i < threadstouse; i++)
			{
				// Create a new thread Info
				SharedPtr<ClothThreadInfo> info = MakeShared<ClothThreadInfo>();

				info->fixedDeltatime = 1.0f / 60.0f;
				info->gravity = Vector3(0.0f, -9.81f, 0.0f);
				info->sleepTime = 1;
				info->ClothHandles;
				info->numberofSubsetps = 16;
				info->run = true;
				info->isAlive = true;

				for (int j = 0; j < bodyPerThread; j++)
				{
					if (bodyCounter < newbodyCount)
					{
						SharedPtr<Cloth> body = clothRefs[bodyCounter];
						body->SetBallRef(ballRef);
						info->ClothHandles.push_back(body);

						bodyCounter++;
					}
				}



				ClothThreadInfo* tsInfo = info.get();

				DWORD ThreadId = 0;
				HANDLE threadHandle = 0;
				threadHandle = CreateThread(
					NULL,					// lpThreadAttributes,
					0,						// dwStackSize,
					UpdateThreadedCloth,		// lpStartAddress,
					(void*)tsInfo,				//  lpParameter,
					0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
					&ThreadId); // lpThreadId


				ThreadId++;

				threadInfoList.push_back(info);
			}

		}

		void ThreadedClothSolver::OnUpdate(const float deltaTime)
		{
			timestepCounter += deltaTime;
			if (timestepCounter < fixedDeltaTime) { return; }

			timestepCounter = 0.0f;
			for (SharedPtr<Cloth>& body : clothRefs)
			{
				body->OnUpdate(deltaTime);
			}
		}

		void ThreadedClothSolver::SetPaused(bool paused)
		{
			for (SharedPtr<Cloth>& body : clothRefs)
			{
				body->SetPaused(paused);
			}
		}
}
}