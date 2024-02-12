#ifndef AIMANAGER_H
#define AIMANAGER_H
#include "SaltnPepperEngine.h"
namespace SaltnPepperEngine
{
	namespace AI
	{

		class AIManager
		{
		public:

			AIManager() {};
			~AIManager() {};

			//void Init();
			void Update(float deltaTime,Transform* playerTransform, Transform* playerLook);

		

			Transform* m_playerTransform = nullptr;
			Transform* m_playerLook = nullptr;
		};

	}
}


#endif // !AIMANAGER_H
