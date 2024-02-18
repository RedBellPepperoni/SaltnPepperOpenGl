#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{
	namespace Rendering
	{
		

		class BaseLight
		{
		protected:
			// The Light affecting the side that is on the opposite of the lit surface
			float ambientIntensity = 0.1f;

			// The actual intensity of the light
			float intensity = 0.6f;

			// The Color of the Light (Defaulted to White)
			Vector3 color{ 1.0f };

		public:

			BaseLight() = default;
			virtual ~BaseLight() = default;

			void SetAmbientIntensity(float newIntensity) { ambientIntensity = newIntensity; }
			void SetIntensity(float newIntensity) { intensity = newIntensity; }
			void SetColor(Vector3 newColor) 
			{ 
				// making sure the color is never in the HDR range // for now
				color = VectorMax(VectorMin(color, Vector3(1.0f)), Vector3(0.0f));
			}

			float GetAmbientIntensity() const { return ambientIntensity; }
			float GetIntensity() const { return intensity; }
			Vector3 GetColor() const { return color; }

		};

	}
}

#endif // !BASELIGHT_H

