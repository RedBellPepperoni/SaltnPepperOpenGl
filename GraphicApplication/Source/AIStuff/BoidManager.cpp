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

		}

		void BoidManager::CreateObstacle(const Vector3& position)
		{
		}

		void BoidManager::CreatePredator(const Vector3& position)
		{
		}



		void BoidManager::OnInit()
		{
			obstacleRadius = 2.0f;
			avoidanceRadius = 5.0f;
			predatorAvoidanceRadius = 12.0f;
		}
		void BoidManager::OnUpdate(const float& deltaTime)
		{
		}
		
		void BoidManager::SeekAI(uint32_t index, Vector2 target)
		{
		}
		void BoidManager::SpreadAI(uint32_t index)
		{
		}
		void BoidManager::AlignAI(uint32_t index)
		{
		}
		void BoidManager::UpdateFlock()
		{
		}
		void BoidManager::GroupAI(uint32_t index)
		{
		}
		bool BoidManager::CheckAvoidanceFOV(uint32_t i, uint32_t j)
		{
			return false;
		}
		bool BoidManager::CheckFOV(uint32_t i, uint32_t j)
		{
			return false;
		}
		void BoidManager::CheckObtacles(uint32_t index)
		{
		}
		void BoidManager::CheckPredator(uint32_t index)
		{
		}
		void BoidManager::AddObstacle(float x, float y)
		{
		}
		void BoidManager::AddOSteeringObject(float x, float y)
		{
		}
		void BoidManager::MoveAttractor(const Vector3& position)
		{
		}
		void BoidManager::ClearObjects()
		{
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