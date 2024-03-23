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
			flockTransform.SetScale(Vector3(0.5f));
			

			// Add a Render Model to the Entity
			ModelComponent& flockModelComp = flockEntity.AddComponent<ModelComponent>("Rooster");
			
			// Set the Material Parameters
			SharedPtr<Material>& flockMat = flockModelComp.m_handle->GetMeshes()[0]->GetMaterial();
			flockMat->SetAlbedoTexture("rooster");

			// Add the Flock Identifier
			Flock& flockComp = flockEntity.AddComponent<Flock>();
			flockComp.flockId = static_cast<uint32_t>(flockSize);

			flockComp.velocity = Vector2(position.x,position.z);

			flockSize++;


		}

		void BoidManager::CreateObstacle(const Vector3& position, bool hideMesh)
		{
			// Dynamic name allocation
			std::string name = "ObstacleEntity_" + std::to_string(obstacleCount);

			// Create the Entity in the Entity system
			Entity obstacleEntity = Application::GetCurrent().GetCurrentScene()->CreateEntity(name);

			Transform& obstacleTransform = obstacleEntity.GetComponent<Transform>();

			obstacleTransform.SetPosition(position);
			obstacleTransform.SetScale(Vector3(1.0f));

			float RandomYaw = Random32::Range.GetRandom(-60.0f, 70.0f);
			obstacleTransform.SetEularRotation(Vector3(0.0f,RandomYaw,0.0f));

			if (!hideMesh)
			{
				// Add a Render Model to the Entity
				ModelComponent& obstacleModelComp = obstacleEntity.AddComponent<ModelComponent>("Rock");

				// Set the Material Parameters
				SharedPtr<Material>& obstacleMat = obstacleModelComp.m_handle->GetMeshes()[0]->GetMaterial();
				obstacleMat->SetAlbedoTexture("rock");

			}
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
			predatorTransform.SetScale(Vector3(0.5f));


			// Add a Render Model to the Entity
			ModelComponent& predatorModelComp = predatorEntity.AddComponent<ModelComponent>("Dog");

			// Set the Material Parameters
			SharedPtr<Material>& predatorMat = predatorModelComp.m_handle->GetMeshes()[0]->GetMaterial();
			predatorMat->SetAlbedoTexture("dog");

			// Add the Flock Identifier
			Predator& predatorComp = predatorEntity.AddComponent<Predator>();
		}



		void BoidManager::OnInit()
		{
			obstacleRadius = 7.0f;
			avoidanceRadius = 3.5f;
			predatorAvoidanceRadius = 16.0f;
		}


		void BoidManager::OnUpdate(const float& deltaTime)
		{
			localdeltaTime = deltaTime;

			ComponentView flockView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Flock>();
			ComponentView obstacleView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Obstacle>();

			ComponentView predatorView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<Predator>();

			if (predatorView.IsEmpty())
			{
				LOG_ERROR("NO PREDATOR FOUND");
				return;
			}

			Transform& predatorTransform = predatorView[0].GetComponent<Transform>();
			Vector3 worldPos = predatorTransform.GetPosition();

			Vector2 predatorPosition = Vector2{ worldPos.x,worldPos.z };
			std::vector<Vector2> obstaclePositionList;

			for (Entity obstacleEntity : obstacleView)
			{
				Vector3 obstacleWorldPos = obstacleEntity.GetComponent<Transform>().GetPosition();
				Vector2 finalpos = Vector2{ obstacleWorldPos.x,obstacleWorldPos.z };
				obstaclePositionList.push_back(finalpos);

			}



			UpdateFlock(flockView, obstaclePositionList, predatorPosition);

			UpdatePredator(flockView, obstaclePositionList, predatorTransform);
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

				float distance = Length(avoidDirection);

				if (distance < avoidanceRadius)
				{
					Vector2 finalAvoidance = distance > 0.1f ? Normalize(avoidDirection) : Vector2{ 0.0f };
					flockRef.avoidanceVec = flockRef.avoidanceVec + finalAvoidance;
				}
			}

			Vector2 finalVector = LengthSquared(flockRef.avoidanceVec) > 0.2f ? Normalize(flockRef.avoidanceVec) : Vector2{0.0f};
			flockRef.avoidanceVec = finalVector * weight * localdeltaTime;
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

				float distance = Length(avoidDirection);

				if (distance < avoidanceRadius * 2)
				{
					Vector2 finalVelo = LengthSquared(flockComp.velocity) > 0.2f ? Normalize(flockComp.velocity) : Vector2{ 0.0f };
					flockRef.alignmentVec = flockRef.alignmentVec + finalVelo;
					neighbours += 1;
				}

				flockRef.alignmentVec = Vector2{ flockRef.alignmentVec.x / neighbours, flockRef.alignmentVec.y / neighbours };
				
				
				Vector2 finalAlign = LengthSquared(flockRef.alignmentVec) > 0.2f ? Normalize(flockRef.alignmentVec) : Vector2{ 0.0f };
				flockRef.alignmentVec = finalAlign * weight * localdeltaTime;

			}
		}

		void BoidManager::UpdateFlock(ComponentView<Flock>& flockView, const std::vector<Vector2>& obstaclePositionList, const Vector2& predatorPos)
		{
			float speed = 6.0f;

			const float XMin = - Bounds.x / 2;
			const float XMax = Bounds.x / 2;
			
			const float YMin = - Bounds.y / 2;
			const float YMax = Bounds.y / 2;

			
			// make teh boids attarct to the center
			Vector2 attractorTarget = Vector2{0.0f};

			

			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();
				Transform& flockTransform = flockEntity.GetComponent<Transform>();

				Vector3 worldPos = flockTransform.GetPosition();
				Vector2 convertedPos = Vector2{ worldPos.x, worldPos.z };

				// Process Agent seeking the Origin
				SeekAI(convertedPos,flockComp, attractorTarget);

				// Process Internal Agent to Agent avoidance
				SpreadAI(convertedPos, flockComp, flockView);

				// Process Agent Aligh=nemnt
				AlignAI(convertedPos,flockComp, flockView);

				// Process Agent Grouping
				GroupAI(convertedPos, flockComp, flockView);

				// Process Agent Object Avoidance
				CheckObstacles(convertedPos, flockComp, obstaclePositionList);

				// Process Agent PredatorAvoidance
				CheckPredator(convertedPos, flockComp, predatorPos);

				// Process Velocity
				flockComp.ProcessVelocity();

				// Clamp Max Speed
				float velocityMagnitude = LengthSquared(flockComp.velocity);

				//LOG_WARN("Velocity = {0}", velocityMagnitude);
				if (velocityMagnitude > (speed *speed))
				{
					flockComp.velocity = Normalize(flockComp.velocity) * speed;
				}

				Vector2 newPosition = convertedPos + flockComp.velocity * localdeltaTime;

				// Bordertraversal === X Direction
				if (newPosition.x > XMax)
				{
					newPosition.x = XMin;
				}

				else if (newPosition.x < XMin)
				{
					newPosition.x = XMax;
				}

				// Bordertraversal === Y Direction
				if (newPosition.y > YMax)
				{
					newPosition.y = YMin;
				}

				else if (newPosition.y < YMin)
				{
					newPosition.y = YMax;
				}


				
				
				flockTransform.SetPosition(Vector3(newPosition.x, worldPos.y,newPosition.y));
			
				UpdateAIRotation(flockTransform,flockComp);
			
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

				float distance = Length(avoidDirection);

				if (distance < avoidanceRadius * 2)
				{
					avgPos = avgPos + otherPosition;
					neighbours += 1;
				}

			}

			if (neighbours > 0)
			{
				avgPos = Vector2{ avgPos.x / neighbours ,avgPos.y / neighbours };
				flockRef.cohesionVec = avgPos - position;

				Vector2 finalCohesion = Length(flockRef.cohesionVec) > 0.2f ? Normalize(flockRef.cohesionVec) : Vector2{ 0.0f };
				flockRef.cohesionVec = finalCohesion * weight * localdeltaTime;
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

				float distanceToObstacle = Length(avoidDirection);
				if (distanceToObstacle < obstacleRadius)
				{
					weight = (obstacleRadius / distanceToObstacle) * maxWeight;

					Vector2 finalAvoidance = distanceToObstacle > 0.01f ? Normalize(avoidDirection * weight) : Vector2{ 0.0f };
					flockRef.obstacleAvoidanceVec = flockRef.obstacleAvoidanceVec + (finalAvoidance);
				}

				Vector2 obstacleAvoid = Length(flockRef.obstacleAvoidanceVec) > 0.01f ? Normalize(flockRef.obstacleAvoidanceVec) : Vector2{ 0.0f };

				flockRef.obstacleAvoidanceVec = obstacleAvoid * baseWeight * localdeltaTime;
			}

		}


		void BoidManager::CheckPredator(const Vector2& position, Flock& flockRef, const Vector2& predatorPosition)
		{
			constexpr float maxWeight = 10.0f;
			constexpr float baseWeight = 10.0f;
			float weight = 0.0f;

			flockRef.predatorAvoidanceVec = Vector2{ 0.0f };
			
			Vector2 avoidDirection = position - predatorPosition;

			float distanceToPredator = Length(avoidDirection);

			if (distanceToPredator < predatorAvoidanceRadius)
			{
				weight = (obstacleRadius / distanceToPredator) * maxWeight;
				Vector2 finalAvoidance = distanceToPredator > 0.2f ? Normalize(avoidDirection * weight) : Vector2{ 0.0f };
				flockRef.predatorAvoidanceVec = flockRef.predatorAvoidanceVec + finalAvoidance;
			}

			Vector2 finalPredatorAvoid = Length(flockRef.predatorAvoidanceVec) > 0.2f ? Normalize(flockRef.predatorAvoidanceVec) : Vector2{ 0.0f };
			flockRef.predatorAvoidanceVec = finalPredatorAvoid * baseWeight * localdeltaTime;


		}

		void BoidManager::UpdatePredator(ComponentView<Flock>& flockView, const std::vector<Vector2>& obstaclePositionList, Transform& predatorTransform)
		{
			//Speed less than the Flocks 
			float speed = 5.5f;

			const float XMin = -Bounds.x / 2;
			const float XMax = Bounds.x / 2;

			const float YMin = -Bounds.y / 2;
			const float YMax = Bounds.y / 2;

			
			Vector3 worldPos = predatorTransform.GetPosition();
			Vector2 predatorPosition = Vector2{ worldPos.x,worldPos.z};

			// Process Predator Seek behaviour
			PredatorSeek(predatorPosition,flockView);

			//Process Obstacle Avoidance
			PredatorCheckObtacles(predatorPosition,obstaclePositionList);

			pVelocity = pVelocity + pTargetVec + pObstacleAvoidanceVec;

			float velocityMagnitude = Length(pVelocity);

			if (velocityMagnitude > speed)
			{
				pVelocity = Normalize(pVelocity) * speed;
			}

			Vector2 newPosition = predatorPosition + pVelocity * localdeltaTime;

			// Bordertraversal === X Direction
			if (newPosition.x > XMax)
			{
				newPosition.x = XMin;
			}

			else if (newPosition.x < XMin)
			{
				newPosition.x = XMax;
			}

			// Bordertraversal === Y Direction
			if (newPosition.y > YMax)
			{
				newPosition.y = YMin;
			}

			else if (newPosition.y < YMin)
			{
				newPosition.y = YMax;
			}


			predatorTransform.SetPosition(Vector3(newPosition.x, worldPos.y, newPosition.y));

			PredatorUpdateRotation(predatorTransform);

		}
	
		void BoidManager::PredatorSeek(const Vector2& position, ComponentView<Flock>& flockView)
		{
			constexpr float weight = 10.0f;

			if (flockView.IsEmpty())
			{
				return;
			}

			Vector2 avgPos = Vector2{ 0.0f };

			for (Entity flockEntity : flockView)
			{
				Flock& flockComp = flockEntity.GetComponent<Flock>();
				Transform& flockTransform = flockEntity.GetComponent<Transform>();

				Vector3 worldPos = flockTransform.GetPosition();
				Vector2 currentPos = Vector2{ worldPos.x, worldPos.z };

				
				avgPos = avgPos + currentPos;
			}

			avgPos = Vector2{ avgPos.x / flockSize ,avgPos.y/flockSize};

			pTargetVec = avgPos - position;

			Vector2 finalTargetVec = Length(pTargetVec) > 0.2f ? Normalize(pTargetVec) : Vector2{ 0.0f };

			pTargetVec = finalTargetVec * weight * localdeltaTime;

		}

		void BoidManager::PredatorCheckObtacles(const Vector2& position,const std::vector<Vector2>& obstaclePositionList)
		{
			constexpr float maxWeight = 25.0f;
			constexpr float baseWeight = 25.0f;
			float weight = 0.0f;

			pObstacleAvoidanceVec = Vector2{ 0.0f };

			for (const Vector2& obstaclePos : obstaclePositionList)
			{
				Vector2 avoidDirection = position - obstaclePos;
				float distanceToObstacle = Length(avoidDirection);

				if (distanceToObstacle < obstacleRadius)
				{
					weight = (obstacleRadius / distanceToObstacle) * maxWeight;

					Vector2 finalAvoidance = distanceToObstacle > 0.01f ? Normalize(avoidDirection * weight) : Vector2{ 0.0f };

					pObstacleAvoidanceVec = pObstacleAvoidanceVec + finalAvoidance;
				}


				Vector2 finalObs = Length(pObstacleAvoidanceVec) > 0.01f ? Normalize(pObstacleAvoidanceVec) : Vector2{ 0.0f };
				pObstacleAvoidanceVec = finalObs * baseWeight * localdeltaTime;
			
			}


		}
		
		void BoidManager::PredatorUpdateRotation(Transform& predatorTransform )
		{
			//float angle = atan2f(pVelocity.y, pVelocity.x);

			Vector3 velocity = Normalize(Vector3{ pVelocity.x,0.0f,pVelocity.y });
			Vector3 worldFront = Vector3{ 0.0f,0.0f,-1.0f };
			float angle = glm::angle(velocity, worldFront);

		
			Quaternion rotation = GetQuaternion(Rotate(Matrix4(1.0f), angle, Vector3(0.0f, 1.0f, 0.0f)));
			
			predatorTransform.SetRotation(rotation);

		}

		void BoidManager::UpdateAIRotation(Transform& flockTransform, Flock& flockRef)
		{
			Vector3 velocity = Normalize(Vector3{ flockRef.velocity.x,0.0f, flockRef.velocity.y });
			Vector3 worldFront = Vector3{ 0.0f,0.0f,-1.0f };
			float angle = glm::angle(velocity, worldFront);


			Quaternion rotation = GetQuaternion(Rotate(Matrix4(1.0f), angle, Vector3(0.0f, 1.0f, 0.0f)));

			flockTransform.SetRotation(rotation);
		}
	}
}