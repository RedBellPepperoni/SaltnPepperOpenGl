#ifndef  DEPRECATED_LIGHT_H
#define  DEPRECATED_LIGHT_H

#include <string>
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
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
			LightType type = LightType::DirectionLight;
			float innerAngle = 0.0f;
			float outerAngle = 0.0f;

			SharedPtr<Texture> depthMap;

			constexpr static size_t TextureCount = 3;
			std::array<float, TextureCount> Projections = { 5.0f, 25.0f, 250.0f };

		private:
			std::array<Matrix4, TextureCount> matrices;

			Matrix4 ComputeCascadeMatrix(size_t cascadeIndex, float aspectRatio, float fov, const Matrix4& viewMatrix) const;

		};

		template <class Archive>
		void serialize(Archive& archive, Light& light)
		{
			archive(light.position, light.color, light.type, light.innerAngle, light.outerAngle, light.direction, light.intensity, light.radius);
		}

	}
}



#endif // ! DEPECRATED_LIGHT_H
