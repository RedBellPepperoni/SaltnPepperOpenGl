#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H
#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{

	namespace Components
	{
		class Transform;
	}

	using namespace Components;

	namespace Audio
	{


		class AudioListener
		{


		public:

			AudioListener();
			~AudioListener() = default;



			void Update(Transform& transform, float deltaTime);


		};

	}
}

#endif // !AUDIOLISTENER_H
