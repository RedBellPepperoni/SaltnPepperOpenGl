#ifndef  LIGHT_H
#define  LIGHT_H

#include <string>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		enum class LightType : uint8_t
		{
			DirectionLight = 0,
			SpotLight = 1,
			PointLight = 2

		};


		struct Light
		{

			static std::string GetStringfromType(const LightType& type)
			{
				switch (type)
				{
				case LightType::DirectionLight:
					return "Directional";

				case LightType::SpotLight:
					return "Spot";

				case LightType::PointLight:
					return "Point";

				default:
					return "UndefinedType";
				}
			}

			static LightType GetTypeFromString(const std::string& lightString)
			{
				if (lightString == "Direction")
				{
					return LightType::DirectionLight;
				}

				else if (lightString == "Spot")
				{
					return LightType::SpotLight;
				}

				else if (lightString == "Point")
				{
					return LightType::PointLight;
				}

				LOG_ERROR("UNsupportedLight Type String");
				return LightType::DirectionLight;
			}

			Vector3 color = Vector3(1.0f);
			Vector3 position = Vector3(0.0f);
			Vector3 direction = Vector3(0.0f);
		

			float intensity = 10.0f;
			float radius = 0.0f;
			LightType type = LightType::PointLight;
			float innerAngle = 0.0f;
			float outerAngle = 0.0f;


		};

		template <class Archive>
		void serialize(Archive& archive, Light& light)
		{
			archive(light.position, light.color, light.type, light.innerAngle, light.outerAngle, light.direction, light.intensity, light.radius);
		}

	}
}



#endif // ! LIGHT_H
