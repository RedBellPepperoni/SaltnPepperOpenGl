#include "SoftBody.h"
#include <iterator>
#include "Engine/Core/Rendering/Geometry/Mesh.h"
#include "Engine/Core/Rendering/RenderDefinitions.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/EntitySystem/Entity.h"
#include "Engine/Core/System/Application/Application.h"
#include "Engine/Core/Scene/Scene.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{

		void SetDiffVector()
		{

		}


		SoftBody::SoftBody(SharedPtr<TetMesh>& tetmesh ,const float& EdgeCompliance, const float& VolumeCompliance)
		{
			numParticles = tetmesh->Vertices.size() / 3;
			numTets = tetmesh->TetIds.size() / 4;

			std::vector<Vertex> renderVertices = std::vector<Vertex>(numParticles);
			
			// Fill in the vertex positions accotding to the given Data
			for (int i = 0; i < numParticles * 3; i +=3)
			{
				Vector3& vertex = vertPositions.emplace_back();
	
				vertex.x = tetmesh->Vertices[i];
				vertex.y = tetmesh->Vertices[i+1];
				vertex.z = tetmesh->Vertices[i+2];		

				renderVertices[i/3].position = vertex;

			}

		
			// Copy over the Vector
			std::copy(vertPositions.begin(), vertPositions.end(),
				std::back_inserter(vertPreviousPositions));


			tetIdList = tetmesh->TetIds;
			edgeIdList = tetmesh->TetEdgeIds;

			// Set the Edge and Volume compliance Values
			edgeCompliance = EdgeCompliance;
			volumeCompliance = VolumeCompliance;

			particleVelocityList = std::vector<Vector3>(numParticles, Vector3(0.0f));
			inverseMassList = std::vector<float>(numParticles, 0.0f);
			edgeLengthList = std::vector<float>(edgeIdList.size()/2, 0.0f);
		

			restVolume = std::vector<float>(numTets, 0.0f);

			// Generate normals
			Mesh::RecalculateNormals(renderVertices, tetmesh->TetSurfaceTriIds);
			
			// Generate the actual render Mesh
			renderMesh = MakeShared<Mesh>(renderVertices,tetmesh->TetSurfaceTriIds);
			
			for (int index = 0; index < numTets; index++)
			{
				float volume = GetTetVolume(index);
				restVolume[index] = volume;

				float inverseMass = volume > 0.0f ? 1.0f / (volume / 4.0f) : 0.0f;

				inverseMassList[tetIdList[4 * index]] += inverseMass;
				inverseMassList[tetIdList[4 * index + 1]] += inverseMass;
				inverseMassList[tetIdList[4 * index + 2]] += inverseMass;
				inverseMassList[tetIdList[4 * index + 3]] += inverseMass;
			}

			for (int index = 0; index < edgeLengthList.size(); index++)
			{
				uint32_t ID_0 = edgeIdList[2 * index];
				uint32_t ID_1 = edgeIdList[2 * index + 1];
				edgeLengthList[index] = Distance(vertPositions[ID_0], vertPositions[ID_1]);
			}


		}


		void SoftBody::OnInit(const Vector3& position)
		{
			
			
			for (int index = 0; index < numParticles; index++)
			{
				vertPositions[index] += position;
				vertPreviousPositions[index] += position;
			}


			UpdateMesh();

		}


		void SoftBody::UpdateMesh()
		{
			std::vector<Vertex> renderVertices = std::vector<Vertex>(numParticles);

			for (int index = 0; index < vertPositions.size(); index++)
			{
				//Vertex& updatedVertex = renderVertices.emplace_back();
				//updatedVertex.position = vertPositions[index];
				renderVertices[index].position = vertPositions[index];
				
			}

			renderVertices;
			// Recalculat ethe Normals
			Mesh::RecalculateNormals(renderVertices, renderMesh->GetIndexData());

			// Update The VBO
			renderMesh->GetVBO()->SetSubData(0, sizeof(Vertex) * renderVertices.size(),renderVertices.data());
			renderMesh->GetVBO()->UnBind();

		}

		
		void SoftBody::ComputeSkinningInfo(std::vector<Vector3>& visualVertices)
		{
		}

		float SoftBody::GetTetVolume(int index)
		{
			uint32_t index0 = tetIdList[4 * index];
			uint32_t index1 = tetIdList[4 * index + 1];
			uint32_t index2 = tetIdList[4 * index + 2];
			uint32_t index3 = tetIdList[4 * index + 3];

			Vector3 temp0 = vertPositions[index1] - vertPositions[index0];
			Vector3 temp1 = vertPositions[index2] - vertPositions[index0];
			Vector3 temp2 = vertPositions[index3] - vertPositions[index0];
			Vector3 temp3 = Cross(temp0,temp1);
		
			return Dot(temp3, temp2) / 6.0f;	
		}

		void SoftBody::PreSolve(const float& deltaTime, const Vector3& gravity)
		{
			//if (isPaused) { return; }

			for (int index = 0; index < numParticles; index++)
			{
				// Ignore particles with Infinite mass
				if (inverseMassList[index] == 0.0f)
				{
					continue;
				}

				// Add gravity to the particles
				particleVelocityList[index] += gravity * deltaTime;

				// Store the current particle position in to the previous position list 
 				vertPreviousPositions[index] = vertPositions[index];

				// Apply velocity to the particle
				vertPositions[index] += particleVelocityList[index] * deltaTime;

				if (vertPositions[index].y < 0.0f)
				{
					vertPositions[index].y = 0.0f;
				}	 
			}
		}

		void SoftBody::Solve(const float& deltaTime)
		{
			//if (isPaused) { return; }

			SolveEdges(deltaTime);
			SolveVolumes(deltaTime);
		}

		void SoftBody::PostSolve(const float& deltaTime)
		{
			//if (isPaused) { return; }

			for (int index = 0; index < numParticles; index++)
			{
				if (inverseMassList[index] == 0.0f) { continue; }

				particleVelocityList[index] = (vertPositions[index] - vertPreviousPositions[index]) * (1.0f / deltaTime);
				
			}

			//UpdateMesh();
		}

		void SoftBody::SolveEdges(const float& deltaTime)
		{
			float alpha = (edgeCompliance / deltaTime) / deltaTime;

			for (int index = 0; index < edgeLengthList.size(); index++)
			{
				uint32_t index_0 = edgeIdList[2 * index];
				uint32_t index_1 = edgeIdList[2 * index + 1];

				const float weight_0 = inverseMassList[index_0];
				const float weight_1 = inverseMassList[index_1];

				const float weight = (weight_0 + weight_1);
				if (weight == 0.0f) { continue; }

				Vector3 grad = vertPositions[index_0] - vertPositions[index_1];
				float length = Length(grad);

				if (length == 0.0f) { continue; }
				
				grad *= (1.0f / length);

				float restLength = edgeLengthList[index];
				float Coeff = length - restLength;
				float s = -Coeff / (weight + alpha);

				vertPositions[index_0] += grad * s * weight_0;
				vertPositions[index_1] += grad * -s * weight_1;

			}
		}

		void SoftBody::SolveVolumes(const float& deltaTime)
		{
			float alpha = (volumeCompliance / deltaTime) / deltaTime; 

			for (int i = 0; i < numTets; i++)
			{
				float weight = 0.0f;

				std::vector<Vector3> grads = std::vector<Vector3>(4);

				for (int j = 0; j < 4; j++)
				{
					uint32_t index_0 = tetIdList[4 * i + volumeIdOrder[j][0]];
					uint32_t index_1 = tetIdList[4 * i + volumeIdOrder[j][1]];
					uint32_t index_2 = tetIdList[4 * i + volumeIdOrder[j][2]];
				
					Vector3 temp_0 = vertPositions[index_1] - vertPositions[index_0];
					Vector3 temp_1 = vertPositions[index_2] - vertPositions[index_0];
				
					
					grads[j] = Cross(temp_0, temp_1);
					grads[j] *= (1.0f / 6.0f);
	
					weight += inverseMassList[tetIdList[4 * i + j]] * LengthSquared(grads[j]);
				}

				if (weight == 0.0f) { continue; }

				float vol = GetTetVolume(i);
				float restVol = restVolume[i];

				float Coeff = vol - restVol;
				float s = -Coeff / (weight + alpha);

				for (int j = 0; j < 4; j++)
				{
					uint32_t id = tetIdList[4 * i + j];
					vertPositions[id] += grads[j] * s * inverseMassList[id];
				}

			}
		}
		Solver::Solver()
		{
		}
		Solver::~Solver()
		{
		}

		void Solver::OnUpdate(const float& deltaTime)
		{
			if (isPaused) { return; }

			timestepCounter += deltaTime;

			if(timestepCounter < fixedDeltaTime)
			{
				return;
			}

			timestepCounter = 0.0f;

			ComponentView softBodyView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<SoftBodyComponent>();

			std::vector<SharedPtr<SoftBody>> softbodyList;

			float subDeltatime = fixedDeltaTime / numSubsteps;

			for (Entity softEntity : softBodyView)
			{
				SharedPtr<SoftBody>& softBody = softEntity.GetComponent<SoftBodyComponent>().softBodyhandle;
				softbodyList.push_back(softBody);
			}


			for (int step = 0; step < numSubsteps; step++)
			{
				
				for (SharedPtr<SoftBody>& body : softbodyList)
				{
					body->PreSolve(subDeltatime);
					body->Solve(subDeltatime);
					body->PostSolve(subDeltatime);
				}

				/*for (SharedPtr<SoftBody>& body : softbodyList)
				{
					
				
				}

				for (SharedPtr<SoftBody>& body : softbodyList)
				{
					
				}*/
			}

			

		}

		Hash::Hash(int newspacing, uint32_t maxnumObjects)
		{
			spacing = newspacing;
			tableSize = 2 * maxnumObjects;
			cellStart = std::vector<int>(tableSize + 1,0);
			cellEntries = std::vector<int>(maxnumObjects,0);
			queryIds = std::vector<int>(maxnumObjects,0);

			querySize = 0;
		}

		int Hash::HashCords(int xi, int yi, int zi)
		{
			int hash = ((xi * 92837111) ^ (yi * 689287499) ^ (zi* 283923481));
			
			return abs(hash) % tableSize;
		}

		int Hash::IntCoords(const float& axisdata)
		{
			return floor(axisdata / spacing);
		}

		int Hash::HashPosition(const Vector3& data)
		{
			return HashCords(IntCoords(data.x), IntCoords(data.y), IntCoords(data.z));

		}

		void Hash::Create(const std::vector<Vector3>& positions)
		{
			size_t numObjects = min(positions.size(), cellEntries.size());
			
			// Determin the cell sizes
			for (int index = 0; index < numObjects; index++)
			{
				int hash = HashPosition(positions[index]);
				cellStart[hash]++;
			}

			// Determine the cell start

			int start = 0;
			for (int index = 0; index < tableSize; index++)
			{
				start += cellStart[index];
				cellStart[index] = start;
			}
			// Guard clause
			cellStart[tableSize] = start;

			// Finally fill in the Object Ids

			for (int index = 0; index < numObjects; index++)
			{
				int hash = HashPosition(positions[index]);
				cellStart[hash]--;
				cellEntries[cellStart[hash]] = index;
			}

		}

		void Hash::Query(const Vector3& position, const float& maxDistance)
		{
			int x0 = IntCoords(position.x - maxDistance);
			int y0 = IntCoords(position.y - maxDistance);
			int z0 = IntCoords(position.z - maxDistance);

			int x1 = IntCoords(position.x + maxDistance);
			int y1 = IntCoords(position.y + maxDistance);
			int z1 = IntCoords(position.z + maxDistance);

			querySize = 0;

			for (int xi = x0; xi <= x1; xi++) 
			{
				for (int yi = y0; yi <= y1; yi++) 
				{
					for (int zi = z0; zi <= z1; zi++) 
					{
						int hash = HashCords(xi, yi, zi);
						int start = cellStart[hash];
						int end = cellStart[hash + 1];

						for (int index = start; index < end; index++) {
							queryIds[querySize] = cellEntries[index];
							querySize++;
						}
					}
				}
			}

		}



		ThreadedSolver::ThreadedSolver(const int maxThreadcount)
		{

		}

		ThreadedSolver::~ThreadedSolver()
		{

		}

		void ThreadedSolver::OnInit()
		{
			ComponentView softBodyView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<SoftBodyComponent>();

			for (Entity softComp : softBodyView)
			{
				// Get the softbody handle from the compoenet
				SharedPtr<SoftBody> bodyref = softComp.GetComponent<SoftBodyComponent>().softBodyhandle;
				// Store the reference for faster access
				softBodies.push_back(bodyref);
			}
		}

		DWORD WINAPI UpdateThreadedSoftBody(LPVOID lpParameter)
		{
			SoftBodyThreadInfo* threadInfo = (SoftBodyThreadInfo*)lpParameter;



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

						for (int i = 0; i < 12; i++)
						{
							for (SharedPtr<SoftBody>& body : threadInfo->softBodyHandles)
							{
								body->PreSolve(substepDelta);
								body->Solve(substepDelta);
								body->PostSolve(substepDelta);
							}
							
						}

						
					}

					
					//Sleep(threadInfo->sleepTime);
					Sleep(0);
					

				}
			}

			return 0;
		}


		// call this function only Once
		void ThreadedSolver::SetupSoftBodyThreads()
		{
			int softBodyCount = (int)softBodies.size();

			//int bodyPerThread = softBodyCount / maximumThreadCount;
			
			if (softBodyCount > maximumThreadCount * bodyPerThread)
			{
				int difference = (maximumThreadCount * bodyPerThread) - softBodyCount;

				LOG_CRITICAL("Large Number of SoftBodies : deleting {0} out of {1}", difference, softBodyCount);


				softBodies.resize(maximumThreadCount * bodyPerThread);
			}
			

			

			int newbodyCount = (int)(softBodies).size();

			int threadstouse = newbodyCount % bodyPerThread == 0 ? newbodyCount / bodyPerThread : newbodyCount / bodyPerThread +1;
			int bodyCounter = 0;

			for(int i = 0; i < threadstouse; i++)
			{
				// Create a new thread Info
				SharedPtr<SoftBodyThreadInfo> info = MakeShared<SoftBodyThreadInfo>();

				info->fixedDeltatime = 1.0f / 60.0f;
				info->gravity = Vector3(0.0f, -9.81f, 0.0f);
				info->sleepTime = 1;
				info->softBodyHandles;
				info->run = true;
				info->isAlive = true;

				for (int j = 0; j < bodyPerThread; j++)
				{
					if (bodyCounter < newbodyCount)
					{
						SharedPtr<SoftBody> body = softBodies[bodyCounter];
						info->softBodyHandles.push_back(body);
						bodyCounter++;
					}
				}
			

				
				SoftBodyThreadInfo* tsInfo = info.get();

				DWORD ThreadId = 0;
				HANDLE threadHandle = 0;
				threadHandle = CreateThread(
					NULL,					// lpThreadAttributes,
					0,						// dwStackSize,
					UpdateThreadedSoftBody,		// lpStartAddress,
					(void*)tsInfo,				//  lpParameter,
					0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
					&ThreadId); // lpThreadId


				ThreadId++;

				threadInfoList.push_back(info);
			}

			
		}

		void ThreadedSolver::OnUpdate(const float deltaTime)
		{
			timestepCounter += deltaTime;
			if (timestepCounter < fixedDeltaTime) { return; }

			timestepCounter = 0.0f;
			for (SharedPtr<SoftBody>& body : softBodies)
			{
				body->UpdateMesh();
			}
		}

		void ThreadedSolver::SetPaused(bool paused)
		{
			for (SharedPtr<SoftBody>& body : softBodies)
			{
				body->SetPaused(paused);
			}
		}

		
	}
}