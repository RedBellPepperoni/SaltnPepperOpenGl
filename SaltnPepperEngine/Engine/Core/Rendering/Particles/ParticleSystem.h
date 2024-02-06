#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Utils/Serialization/Serialization.h"
#include "Engine/Core/Rendering/Textures/Texture.h"
#include "Engine/Core/Rendering/Buffers/ShaderStorageBuffer.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"


namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	using namespace Components;

	namespace Rendering
	{
		



		class ParticleSystem
		{
		public:

			struct Particle
			{

				Vector3 Position;
				Vector3 Velocity;
				//Vector4 Color;
				Vector3 AngularParams;

				float Lifetime;
				float Size;

				float SpawnDistance;

			};


			enum class EmitterShape : uint8_t
			{
				SPHERE,
				HEMISPHERE,
				DISK,
				RAY,
				LINE,
				CROSS,
				AXIS
			};

			/*enum ParticleBlend : uint8_t
			{
				Additive = 0,
				Alpha = 1,
				Off = 2

			};*/

			
			ParticleSystem() = default;
			~ParticleSystem();
			

			void Update(float deltaTime, Transform& transform);
			void Invalidate();

			size_t GetParticleAllocationOffset() const;
			size_t GetParticleAllocationCount() const;

			size_t GetMaxCount() const;
			void SetMaxCount(size_t count);

			void SetLifetime(float lifetime);
			float GetLifetime() const;

			float GetMinSize() const;
			void SetMinSize(float size);
			float GetMaxSize() const;
			void SetMaxSize(float size);

			float GetSpeed() const;
			void SetSpeed(float speed);
			float GetAngularSpeed() const;
			void SetAngularSpeed(float speed);

			float GetMinSpawnDistance() const;
			void SetMinSpawnDistance(float distance);
			float GetMaxSpawnDistance() const;
			void SetMaxSpawnDistance(float distance);

			float GetMaxInitialTimeAlive() const;
			void SetMaxInitialTimeAlive(float timeAlive);

			bool IsRelative() const;
			void SetRelative(bool relative);

			float GetFading() const;
			void SetFading(float fading);

			EmitterShape GetShape() const;
			void SetShape(EmitterShape shape);

			SharedPtr<ShaderStorageBuffer>& GetSSBO();


		private:

			void FillParticleData(std::vector<Particle>& particleList, Transform& transform) const;

		private:

			SharedPtr<ShaderStorageBuffer> m_SSBO;
			size_t m_particleAllocationOffset = 0;
			size_t m_particleAllocationCount = 0;
			size_t m_maxParticleCount = 1024;
			float m_particleLifetime = 1.0f;
			float m_particleMinSize = 0.05f;
			float m_particleMaxSize = 0.05f;
			float m_particleSpeed = 1.0f;
			float m_particleAngularSpeed = 0.0f;
			float m_maxInitialTimeAlive = 0.0f;
			float m_minSpawnDistance = 0.0f;
			float m_maxSpawnDistance = 0.0f;
			float m_isfading = 0.0f;
			EmitterShape m_shape = EmitterShape::SPHERE;
			bool m_isDirty = true;
			bool m_isRelative = false;

		};

	}
}


#endif // !PARTICLESYSTEM_H

