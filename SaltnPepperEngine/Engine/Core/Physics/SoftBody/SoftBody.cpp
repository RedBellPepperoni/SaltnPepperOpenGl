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


		SoftBody::SoftBody(SharedPtr<TetMesh>& tetmesh, const float& EdgeCompliance, const float& VolumeCompliance)
		{
			numParticles = tetmesh->Vertices.size() / 3;
			numTets = tetmesh->TetIds.size() / 4;

			std::vector<Vertex> renderVertices;
			
			// Fill in the vertex positions accotding to the given Data
			for (int i = 0; i < numParticles * 3; i +=3)
			{
				Vector3& vertex = vertPositions.emplace_back();
				Vertex& renderVertex = renderVertices.emplace_back();
	
				vertex.x = tetmesh->Vertices[i];
				vertex.y = tetmesh->Vertices[i+1];
				vertex.z = tetmesh->Vertices[i+2];		

				renderVertex.position = vertex;

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
			
		}


		void SoftBody::OnInit(const Vector3& position)
		{
			for (int index = 0; index < numParticles; index++)
			{
				vertPositions[index] += position;
				vertPreviousPositions[index] += position;
			}

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
				uint32_t ID_1 = edgeIdList[2 * index  + 1];
				edgeLengthList[index] = Distance(vertPositions[ID_0], vertPositions[ID_1]);
			}

		}


		void SoftBody::UpdateMesh()
		{
			std::vector<Vertex> renderVertices;

			for (int index = 0; index < vertPositions.size(); index++)
			{
				Vertex& updatedVertex = renderVertices.emplace_back();
				updatedVertex.position = vertPositions[index];
				
			}
			// Recalculat ethe Normals
			Mesh::RecalculateNormals(renderVertices, renderMesh->GetIndexData());

			// Update The VBO
			renderMesh->GetVBO()->SetSubData(0,renderVertices.size(),renderVertices.data());


		}

		float SoftBody::GetTetVolume(int index)
		{
			int index0 = tetIdList[4 * index];
			int index1 = tetIdList[4 * index +1];
			int index2 = tetIdList[4 * index +2];
			int index3 = tetIdList[4 * index +3];

			Vector3 temp0 = vertPositions[index1] - vertPositions[index0];
			Vector3 temp1 = vertPositions[index2] - vertPositions[index0];
			Vector3 temp2 = vertPositions[index3] - vertPositions[index0];
			Vector3 temp3 = Cross(temp0,temp1);
		
			return Dot(temp3, temp2);	
		}

		void SoftBody::PreSolve(const float& deltaTime, const Vector3& gravity)
		{
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
			SolveEdges(deltaTime);
			SolveVolumes(deltaTime);
		}

		void SoftBody::PostSolve(const float& deltaTime)
		{
			for (int index = 0; index < numParticles; index++)
			{
				if (inverseMassList[index] <= 0.001f) { continue; }

				particleVelocityList[index] = vertPositions[index] - vertPreviousPositions[index] * (1.0f / deltaTime);
			}

			UpdateMesh();
		}

		void SoftBody::SolveEdges(const float& deltaTime)
		{
			float alpha = edgeCompliance / deltaTime / deltaTime;

			for (int index = 0; index < edgeLengthList.size(); index++)
			{
				uint32_t index_0 = edgeIdList[2 * index];
				uint32_t index_1 = edgeIdList[2 * index + 1];

				const float weight_0 = inverseMassList[index_0];
				const float weight_1 = inverseMassList[index_1];
				const float weight = (weight_0 + weight_1);
				if (weight <= 0.001f) { continue; }

				Vector3 grad = vertPositions[index_0] - vertPositions[index_1];
				float length = Length(grad);

				if (length <= 0.001f) { continue; }
				
				grad *= 1.0f / length;

				float restLength = edgeLengthList[index];
				float Coeff = length - restLength;
				float s = -Coeff / (weight + alpha);

				vertPositions[index_0] += (grad * s * weight_0);
				vertPositions[index_1] += (grad * -s * weight_1);

			}
		}

		void SoftBody::SolveVolumes(const float& deltaTime)
		{
			float alpha = volumeCompliance / deltaTime / deltaTime;

			for (int i = 0; i < numTets; i++)
			{
				float weight = 0.0f;

				std::vector<Vector3> grads;

				for (int j = 0; j < 4; j++)
				{
					uint32_t index_0 = tetIdList[4 * i + volumeIdOrder[j][0]];
					uint32_t index_1 = tetIdList[4 * i + volumeIdOrder[j][1]];
					uint32_t index_2 = tetIdList[4 * i + volumeIdOrder[j][2]];
				
					Vector3 temp_0 = vertPositions[index_1] - vertPositions[index_0];
					Vector3 temp_1 = vertPositions[index_2] - vertPositions[index_0];
				
					Vector3& newgrad = grads.emplace_back();

					newgrad = Cross(temp_0, temp_1);
					newgrad *= 1.0f/6.0f;
	
					weight = inverseMassList[tetIdList[4 * i + j]] * LengthSquared(newgrad);
				}

				if (weight <= 0.001f) { continue; }

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
				SharedPtr<SoftBody> softBody = softEntity.GetComponent<SoftBodyComponent>().softBodyhandle;
				softbodyList.push_back(softBody);	
			}

			for (SharedPtr<SoftBody>& body : softbodyList)
			{
				body->PreSolve(subDeltatime);
			}

			for (SharedPtr<SoftBody>& body : softbodyList)
			{
				body->Solve(subDeltatime);
			}

			for (SharedPtr<SoftBody>& body : softbodyList)
			{
				body->PostSolve(subDeltatime);
			}

		}
	}
}