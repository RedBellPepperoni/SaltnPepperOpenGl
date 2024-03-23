#include "BoidManager.h"
#include <string>

namespace SaltnPepperEngine
{
	

	namespace AI
	{
		void BoidManager::CreateFlockEntity(const Vector3& position)
		{
			// Dynamic name allocation
			std::string name = "FlockEntity_" + std::to_string(flockSize);

			// Create the Entity in the Entity system
			Entity flockEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);

			Transform& flockTransform = flockEntity.GetComponent<Transform>();

			flockTransform.SetPosition(position);
			flockTransform.SetScale(Vector3(1.0f));
			

			// Add a Render Model to the Entity
			ModelComponent& flockModelComp = flockEntity.AddComponent<ModelComponent>(PrimitiveType::Sphere);
			
			// Set the Material Parameters
			SharedPtr<Material>& flockMat = flockModelComp.m_handle->GetMeshes()[0]->GetMaterial();
			flockMat->SetAlbedoTexture("grass");

			// Add the Flock Identifier
			Flock& flockComp = flockEntity.AddComponent<Flock>();
			flockComp.flockId = static_cast<uint32_t>(flockSize);

			flockComp.velocity = Vector2(9.0f);

			flockSize++;


		}

		void BoidManager::CreateObstacle(const Vector3& position)
		{
			// Dynamic name allocation
			std::string name = "ObstacleEntity_" + std::to_string(obstacleCount);

			// Create the Entity in the Entity system
			Entity obstacleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);

			Transform& obstacleTransform = obstacleEntity.GetComponent<Transform>();

			obstacleTransform.SetPosition(position);
			obstacleTransform.SetScale(Vector3(1.0f));


			// Add a Render Model to the Entity
			ModelComponent& obstacleModelComp = obstacleEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);

			// Set the Material Parameters
			SharedPtr<Material>& obstacleMat = obstacleModelComp.m_handle->GetMeshes()[0]->GetMaterial();
			obstacleMat->SetAlbedoTexture("deer");

			// Add the Flock Identifier
			Obstacle& obstacleComp = obstacleEntity.AddComponent<Obstacle>();

			obstacleCount++;
		}

		void BoidManager::CreatePredator(const Vector3& position)
		{

			// Create the Entity in the Entity system
			Entity predatorEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity("Predator_Entity");

			Transform& predatorTransform = predatorEntity.GetComponent<Transform>();

			predatorTransform.SetPosition(position);
			predatorTransform.SetScale(Vector3(1.0f));


			// Add a Render Model to the Entity
			ModelComponent& predatorModelComp = predatorEntity.AddComponent<ModelComponent>(PrimitiveType::Cube);

			// Set the Material Parameters
			SharedPtr<Material>& predatorMat = predatorModelComp.m_handle->GetMeshes()[0]->GetMaterial();
			predatorMat->SetAlbedoTexture("cat");

			// Add the Flock Identifier
			Predator& predatorComp = predatorEntity.AddComponent<Predator>();
		}



		void BoidManager::OnInit()
		{
			obstacleRadius = 2.0f;
			avoidanceRadius = 16.0f;
			predatorAvoidanceRadius = 12.0f;
		}


		void BoidManager::OnUpdate(const float& deltaTime)
		{
			localdeltaTime = deltaTime;
			UpdateFlock();
		}
		
		void BoidManager::SeekAI(const Vector2& position, Flock& flockRef, Vector2 target)
		{
			constexpr float weight = 5.0f;

			flockRef.targetVec = target - position;
			flockRef.targetVec = Normalize(flockRef.targetVec) * weight * localdeltaTime;
			
		}

		void BoidManager::SpreadAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView)
		{
			constexpr float weight = 15.0f;

			flockRef.avoidanceVec = Vector2{ 0.0f };

			
			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();

				// Dont Check With self
				if (flockComp.flockId == flockRef.flockId)
				{
					continue;
				}


				Vector3 otherWorldPos = flockEntity.GetComponent<Transform>().GetPosition();
				Vector2 otherPosition = Vector2{ otherWorldPos.x,otherWorldPos.z };

				Vector2 avoidDirection = position - otherPosition;

				float distance = LengthSquared(avoidDirection);

				if (distance < avoidanceRadius)
				{
					flockRef.avoidanceVec = flockRef.avoidanceVec + Normalize(avoidDirection);
				}
			}

			flockRef.avoidanceVec = Normalize(flockRef.avoidanceVec) * weight * localdeltaTime;
		}


		void BoidManager::AlignAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView)
		{
			constexpr float weight = 10.0f;

			flockRef.alignmentVec = Vector2{ 0.0f };

			int neighbours = 0;

			

			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();

				// Dont Check With self
				if (flockComp.flockId == flockRef.flockId)
				{
					continue;
				}

				Vector3 otherWorldPos = flockEntity.GetComponent<Transform>().GetPosition();
				Vector2 otherPosition = Vector2{ otherWorldPos.x,otherWorldPos.z };

				Vector2 avoidDirection = position - otherPosition;

				float distance = LengthSquared(avoidDirection);

				if (distance < avoidanceRadius * 2)
				{
					flockRef.alignmentVec = flockRef.alignmentVec + Normalize(flockComp.velocity);
					neighbours += 1;
				}

				flockRef.alignmentVec = Vector2{ flockRef.alignmentVec.x / neighbours, flockRef.alignmentVec.y / neighbours };
				flockRef.alignmentVec = Normalize(flockRef.alignmentVec) * weight * localdeltaTime;

			}
		}

		void BoidManager::UpdateFlock()
		{
			float speed = 3.0f;

			ComponentView flockView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Flock>();
			ComponentView obstacleView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Obstacle>();

			// make teh boids attarct to the center
			Vector2 attractorTarget = Vector2{0.0f};

			std::vector<Vector2> obstaclePositionList;

			for (Entity obstacleEntity : obstacleView)
			{
				Vector3 obstacleWorldPos = obstacleEntity.GetComponent<Transform>().GetPosition();
				Vector2 finalpos = Vector2{ obstacleWorldPos .x,obstacleWorldPos .z};
				obstaclePositionList.push_back(finalpos);

			}

			if (obstaclePositionList.size() < 1)
			{
				//LOG_ERROR("NO Obstacles Found");
			}

			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();
				Transform& flockTransform = flockEntity.GetComponent<Transform>();

				Vector3 worldPos = flockTransform.GetPosition();
				Vector2 convertedPos = Vector2{ worldPos.x, worldPos.z };

				// Process Agent seeking the Origin
				SeekAI(convertedPos,flockComp, attractorTarget);

				// Process Internal Agent to Agent avoidance
				//SpreadAI(convertedPos, flockComp, flockView);

				// Process Agent Aligh=nemnt
			//	AlignAI(convertedPos,flockComp, flockView);

				// Process Agent Grouping
				//GroupAI(convertedPos, flockComp, flockView);

				// Process Agent Object Avoidance
				//CheckObstacles(convertedPos, flockComp, obstaclePositionList);

				// Process Agent PredatorAvoidance


				// Process Velocity
				flockComp.ProcessVelocity();

				// Clamp Max Speed
				float velocityMagnitude = LengthSquared(flockComp.velocity);

				LOG_WARN("Velocity = {0}", velocityMagnitude);
				if (velocityMagnitude > (speed *speed))
				{
					flockComp.velocity = Normalize(flockComp.velocity) * speed;
				}

				Vector2 newPosition = convertedPos + flockComp.velocity * localdeltaTime;

				// Bordertraversal === X Direction
				if (newPosition.x > Bounds.x/2)
				{
					newPosition.x = -Bounds.x / 2;
				}

				else if (newPosition.x < -Bounds.x / 2)
				{
					newPosition.x = Bounds.x / 2;
				}

				// Bordertraversal === Y Direction
				if (newPosition.y > Bounds.y / 2)
				{
					newPosition.y = -Bounds.y / 2;
				}

				else if (newPosition.y < -Bounds.y / 2)
				{
					newPosition.y = Bounds.y / 2;
				}


				
				
				flockTransform.SetPosition(Vector3(newPosition.x, 0.0f,newPosition.y));
			
				
			
			}


		}

		void BoidManager::GroupAI(const Vector2& position, Flock& flockRef, ComponentView<Flock>& flockView)
		{
			constexpr float weight = 5.0f;

			flockRef.cohesionVec = Vector2{ 0.0f };
			int neighbours = 0;
			Vector2 avgPos = Vector2{ 0.0f };

			
			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();

				// Dont Check With self
				if (flockComp.flockId == flockRef.flockId)
				{
					continue;
				}

				Vector3 otherWorldPos = flockEntity.GetComponent<Transform>().GetPosition();
				Vector2 otherPosition = Vector2{ otherWorldPos.x,otherWorldPos.z };

				Vector2 avoidDirection = position - otherPosition;

				float distance = LengthSquared(avoidDirection);

				if (distance < avoidanceRadius * 2)
				{

				}

			}
		}



		void BoidManager::CheckObstacles(const Vector2& position, Flock& flockRef, const std::vector<Vector2>& obstaclePositionList)
		{
			constexpr float maxWeight = 25.0f;
			constexpr float baseWeight = 25.0f;
			float weight = 0.0f;

			flockRef.obstacleAvoidanceVec = Vector2{ 0.0f };

			Vector2 flockPos = position;

			for (Vector2 obtstaclePos : obstaclePositionList)
			{
				Vector2 avoidDirection = position - obtstaclePos;

				float distanceToObstacle = LengthSquared(avoidDirection);
				if (distanceToObstacle < obstacleRadius)
				{
					weight = (obstacleRadius / distanceToObstacle) * maxWeight;
					flockRef.obstacleAvoidanceVec = flockRef.obstacleAvoidanceVec + (Normalize(avoidDirection * weight));
				}

				flockRef.obstacleAvoidanceVec = Normalize(flockRef.obstacleAvoidanceVec) * baseWeight * localdeltaTime;
			}

		}


		void BoidManager::CheckPredator(const Vector2& position, Flock& flockRef, const Vector2& predatorPosition)
		{
			constexpr float maxWeight = 10.0f;
			constexpr float baseWeight = 10.0f;
			float weight = 0.0f;

			flockRef.predatorAvoidanceVec = Vector2{ 0.0f };
			
			Vector2 avoidDirection = position - predatorPosition;

			float distanceToPredator = LengthSquared(avoidDirection);

			if (distanceToPredator < predatorAvoidanceRadius)
			{
				weight = (obstacleRadius / distanceToPredator) * maxWeight;
				flockRef.predatorAvoidanceVec = flockRef.predatorAvoidanceVec + (Normalize(avoidDirection * weight));
			}

			flockRef.predatorAvoidanceVec = Normalize(flockRef.predatorAvoidanceVec) * baseWeight * localdeltaTime;


		}

		
	
		void BoidManager::PredatorSeek()
		{
		}

		void BoidManager::PredatorCheckObtacles()
		{
		}
		void BoidManager::UpdatePredator()
		{
		}
		void BoidManager::PredatorUpdateRotation()
		{
		}
		void BoidManager::UpdateAIRotation(uint32_t index)
		{
		}
	}
}