#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Engine/Core/Rendering/Lights/BaseLight.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		class Texture;

		class DirectionalLight : public BaseLight
		{
		public:

			constexpr static size_t TextureCount = 3;

		private:

			std::array<Matrix4, TextureCount> matrices;

			Matrix4 ComputeCascadeMatrix(size_t cascadeIndex, float aspectRatio, float fov, const Matrix4& viewMatrix) const;
		public:


			DirectionalLight();
			void UpdateLightCascades(float aspectRatio, float fov, const Matrix4& viewMatrix);

			const Matrix4& GetMatrix(size_t index) const;

			SharedPtr<Texture> DepthMap;
			Vector3 Direction = Vector3(0.0f, 1.0f, 0.0f);
			std::array<float, TextureCount> Projections = { 5.0f, 25.0f, 250.0f };
			float DepthScale = 20.0f;


		};

	}
}


#endif // !DIRECTIONALLIGHT_H

