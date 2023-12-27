#ifndef SKYBOX_H
#define SKYBOX_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"

namespace SaltnPepperEngine
{

	namespace Rendering
	{

		class CubeMap;

		class Skybox
		{
		public:

			constexpr static float Defaultintensity = 0.5f;

		private:

			Vector3 m_rotation = Vector3(0.0f);
			float m_intensity = Skybox::Defaultintensity;

		public:

			Skybox() = default;

			SharedPtr<CubeMap> cubeMap;

		public:

			void SetIntensity(float intensity);
			const float GetIntensity() const;

			Quaternion GetRotaionQuat() const;
			void SetRotation(const Quaternion& quat);

			void RotateX(float angle);
			void RotateY(float angle);
			void RotateZ(float angle);
			void Rotate(const Vector3& angles);

			const Vector3& GetRotation() const;

			void SetRotation(const Vector3& angles);

		};


	}
}


#endif // !SKYBOX_H
