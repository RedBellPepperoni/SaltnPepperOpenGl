#include "ParticleSystem.h"
#include "Engine/Core/Components/Transform.h"
#include "Engine/Core/EntitySystem/EntityManager.h"
#include "Engine/Utils/Maths/Random.h"

namespace SaltnPepperEngine
{
	namespace Rendering
	{
		ParticleSystem::~ParticleSystem()
		{
			if (m_particleAllocationCount != 0)
			{
				// Clear the SSBO 
			}
		}

		void ParticleSystem::Update(float deltaTime, Transform& transform)
		{
			if (m_isDirty)
			{
				//Create a new Set
				std::vector<Particle> initialState(GetMaxCount());
				FillParticleData(initialState, transform);

				if (m_particleAllocationCount != 0)
				{
					// Overriting Buffer Data or clear It

				}

				m_SSBO->BufferSubData((uint8_t*)initialState.data(), GetMaxCount(), 0);

				m_isDirty = false;
			
			}
		}

		void ParticleSystem::Invalidate()
		{
			m_isDirty = true;
		}

		size_t ParticleSystem::GetParticleAllocationOffset() const
		{
			return m_particleAllocationOffset;
		}
		size_t ParticleSystem::GetParticleAllocationCount() const
		{
			return m_particleAllocationCount;
		}

		size_t ParticleSystem::GetMaxCount() const
		{
			return m_maxParticleCount;
		}

		void ParticleSystem::SetMaxCount(size_t count)
		{
			m_maxParticleCount = count;
			Invalidate();
		}

		void ParticleSystem::SetLifetime(float lifetime)
		{
			m_particleLifetime = Max(lifetime,0.0f);
			Invalidate();

		}

		float ParticleSystem::GetLifetime() const
		{
			return m_particleLifetime;
		}

		float ParticleSystem::GetMinSize() const
		{
			return m_particleMinSize;
		}

		void ParticleSystem::SetMinSize(float size)
		{
			m_particleMinSize = Clamp(size,0.0f,GetMaxSize());
			Invalidate();
		}

		float ParticleSystem::GetMaxSize() const
		{
			return m_particleMaxSize;
		}

		void ParticleSystem::SetMaxSize(float size)
		{
			m_particleMaxSize = Max(size, 0.0f);
			SetMinSize(GetMinSize());
			
		}

		float ParticleSystem::GetSpeed() const
		{
			return m_particleSpeed;
		}

		void ParticleSystem::SetSpeed(float speed)
		{
			m_particleSpeed = speed;
			Invalidate();
		}

		float ParticleSystem::GetAngularSpeed() const
		{
			return m_particleAngularSpeed;
		}

		void ParticleSystem::SetAngularSpeed(float speed)
		{
			m_particleAngularSpeed = speed;
			Invalidate();
		}

		float ParticleSystem::GetMinSpawnDistance() const
		{
			return m_minSpawnDistance;
		}

		void ParticleSystem::SetMinSpawnDistance(float distance)
		{
			m_minSpawnDistance = Clamp(distance, 0.0f, GetMaxSpawnDistance());
			Invalidate();
		}

		float ParticleSystem::GetMaxSpawnDistance() const
		{
			return m_maxSpawnDistance;
		}

		void ParticleSystem::SetMaxSpawnDistance(float distance)
		{
			m_maxSpawnDistance = Max(distance, 0.0f);
			SetMinSpawnDistance(GetMinSpawnDistance());

		}

		float ParticleSystem::GetMaxInitialTimeAlive() const
		{
			return m_maxInitialTimeAlive;
		}

		void ParticleSystem::SetMaxInitialTimeAlive(float timeAlive)
		{
			m_maxInitialTimeAlive = Max(timeAlive, 0.0f);
			Invalidate();
		}

		bool ParticleSystem::IsRelative() const
		{
			return m_isRelative;
		}

		void ParticleSystem::SetRelative(bool relative)
		{
			m_isRelative = relative;
			Invalidate();
		}

		float ParticleSystem::GetFading() const
		{
			return m_isfading;
		}

		void ParticleSystem::SetFading(float fading)
		{
			m_isfading = Max(fading, 0.0f);
		}

		ParticleSystem::EmitterShape ParticleSystem::GetShape() const
		{
			return m_shape;
		}

		void ParticleSystem::SetShape(EmitterShape shape)
		{
			m_shape = shape;
			Invalidate();
		}

		SharedPtr<ShaderStorageBuffer>& ParticleSystem::GetSSBO()
		{
			return m_SSBO;
		}

		Vector3 GetRandomVector3(ParticleSystem::EmitterShape shape)
		{
			Vector3 result = Vector3{0.0f};
			switch (shape)
			{
			case ParticleSystem::EmitterShape::SPHERE:
			{
				result = Random32::Range.GetRandomUnitVector3();
				break;
			}
			case ParticleSystem::EmitterShape::HEMISPHERE:
			{
				result = Random32::Range.GetRandomUnitVector3();
				result.y = std::abs(result.y);
				break;
			}
			case ParticleSystem::EmitterShape::DISK:
			{
				Vector2 v = Random32::Range.GetRandomUnitVector2();
				result = Vector3(v.x, 0.0f, v.y);
				break;
			}
			case ParticleSystem::EmitterShape::RAY:
			{
				result = Vector3(0.0f, 1.0f, 0.0f);
				break;
			}
			case ParticleSystem::EmitterShape::LINE:
			{
				result = Vector3(0.0f, Random32::Range.GetRandomBool() ? 1.0f : -1.0f, 0.0f);
				break;
			}

			case ParticleSystem::EmitterShape::CROSS:
			{
				int rng = Random32::Range.GetRandom(0,3);
				switch (rng)
				{
				case 0: result = Vector3(1.0f, 0.0f, 0.0f); break;
				case 1: result = Vector3(-1.0f, 0.0f, 0.0f); break;
				case 2: result = Vector3(0.0f, 0.0f, 1.0f); break;
				case 3: result = Vector3(0.0f, 0.0f, -1.0f); break;
				}
				break;
			}
			case ParticleSystem::EmitterShape::AXIS:
			{
				int rng = Random32::Range.GetRandom(0, 5);
				switch (rng)
				{
				case 0: result = Vector3(1.0f, 0.0f, 0.0f); break;
				case 1: result = Vector3(-1.0f, 0.0f, 0.0f); break;
				case 2: result = Vector3(0.0f, 1.0f, 0.0f); break;
				case 3: result = Vector3(0.0f, -1.0f, 0.0f); break;
				case 4: result = Vector3(0.0f, 0.0f, 1.0f); break;
				case 5: result = Vector3(0.0f, 0.0f, -1.0f); break;
				}
				break;
			}
			default:
				result = Vector3(0.0f, 0.0f, 0.0f);
			}
			return result;
		}


		void ParticleSystem::FillParticleData(std::vector<Particle>& particleList, Transform& transform) const
		{
			Matrix4 particleMatrix = IsRelative() ? Matrix4(1.0f) : transform.GetMatrix();

			for (Particle& particle : particleList)
			{
				Vector3 shapeBasedRandom = Matrix3(particleMatrix) * GetRandomVector3(GetShape());
				float linearSpeed = GetSpeed() == 0.0f ? 0.0001f : GetSpeed();

				particle.AngularParams = Matrix3(particleMatrix) * Vector3(0.0f, 1.0f, 0.0f) * m_particleAngularSpeed;

				particle.SpawnDistance = Random32::Range.GetRandom(GetMinSpawnDistance(), GetMaxSpawnDistance());
				particle.Position = particle.SpawnDistance * shapeBasedRandom + Vector3(particleMatrix[3]);
				particle.Velocity = linearSpeed * shapeBasedRandom;
				particle.Lifetime = Random32::Range.GetRandom(0.0f,1.0f) * GetMaxInitialTimeAlive();
				particle.Size = Random32::Range.GetRandom(GetMinSize(), GetMaxSize());
			
			}
		}
	}
}