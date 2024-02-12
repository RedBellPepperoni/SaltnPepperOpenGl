#include "AIManager.h"
#include "AIBaseAgent.h"


namespace SaltnPepperEngine
{
	namespace AI
	{
		/*void AIManager::Init(Transform* playerTransform, Transform* playerLook)
		{
			m_playerTransform = playerTransform;
			m_playerLook = playerLook;

		}*/


		void AIManager::Update(float deltaTime, Transform* playerTransform, Transform* playerLook)
		{
			ComponentView aiView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<AIBaseAgent>();

			Vector3 playerPosition = playerTransform->GetPosition();
			Vector3 playerDirection = Normalize(playerLook->GetForwardVector());

			// LOG_WARN("{0} : {1}", playerPosition.x, playerPosition.z);

			for (Entity aiEntity : aiView)
			{
				AIBaseAgent& baseAgent = aiEntity.GetComponent<AIBaseAgent>();
				baseAgent.Update(deltaTime,playerPosition,playerDirection);
			}

		}
	}
}