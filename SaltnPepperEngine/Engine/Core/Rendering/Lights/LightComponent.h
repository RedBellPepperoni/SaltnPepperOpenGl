#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "BaseLight.h"
#include "DirectionalLight.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{	
		enum class LightType : uint8_t
		{
			DIRECTIONAL = 0,
			SPOT = 1,
			POINT = 2
		};


		class LightComponent
		{
		public:


			LightComponent();
			LightComponent(LightType lighttype);
			virtual ~LightComponent();

			void SetLightType(LightType lighttype);

			SharedPtr<BaseLight> GetLight();

		protected:
			SharedPtr<BaseLight> light;
			LightType type = LightType::POINT;
		};
	}
}

#endif // !LIGHTCOMPONENT_H

