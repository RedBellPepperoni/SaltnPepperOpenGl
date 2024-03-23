#include "AudioListener.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Core/Components/Transform.h"
#include "AudioManager.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	namespace Audio
	{
		
		AudioListener::AudioListener()
		{
		}


		void AudioListener::Update(Transform& transform,float deltaTime)
		{
			
			// Getting the actual forward vector
			Vector3 finalDirection = transform.GetRotation() * Vector3(0.0f, 0.0f, 1.0f);
			// Removing the Up/Down direction
			finalDirection.y = 0;

			// Sending the data to Audio manager;
			AudioManager::GetInstance().SetListenerAttributes(transform.GetPosition(), Vector3(0.0f), finalDirection, Vector3(0.0f, 1.0f, 0.0f));

		}



	}
}