#include "LightComponent.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		LightComponent::LightComponent()
		{
			SetLightType(LightType::POINT);
		}

		LightComponent::LightComponent(LightType lighttype)
		{
			SetLightType(lighttype);
		}

		LightComponent::~LightComponent()
		{

		}

		void LightComponent::SetLightType( LightType lighttype)
		{
			switch (lighttype)
			{
			case SaltnPepperEngine::Rendering::LightType::DIRECTIONAL: light = MakeShared<DirectionalLight>();
				break;
			case SaltnPepperEngine::Rendering::LightType::SPOT:
				break;
			case SaltnPepperEngine::Rendering::LightType::POINT:
				break;
			default:
				break;
			}

			type = lighttype;
		}
		SharedPtr<BaseLight> LightComponent::GetLight()
		{
			return light;
		}
	}
}