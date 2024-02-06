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
			return size_t();
		}
		size_t ParticleSystem::GetParticleAllocationCount() const
		{
			return size_t();
		}
		size_t ParticleSystem::GetMaxCount() const
		{
			return size_t();
		}
		void ParticleSystem::SetMaxCount(size_t count)
		{
		}
		void ParticleSystem::SetLifetime(float lifetime)
		{
		}
		float ParticleSystem::GetLifetime() const
		{
			return 0.0f;
		}
		float ParticleSystem::GetMinSize() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetMinSize(float size)
		{
		}
		float ParticleSystem::GetMaxSize() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetMaxSize(float size)
		{
		}
		float ParticleSystem::GetSpeed() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetSpeed(float speed)
		{
		}
		float ParticleSystem::GetAngularSpeed() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetAngularSpeed(float speed)
		{
		}
		float ParticleSystem::GetMinSpawnDistance() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetMinSpawnDistance(float distance)
		{
		}
		float ParticleSystem::GetMaxSpawnDistance() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetMaxSpawnDistance(float distance)
		{
		}
		float ParticleSystem::GetMaxInitialTimeAlive() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetMaxInitialTimeAlive(float timeAlive)
		{
		}
		bool ParticleSystem::IsRelative() const
		{
			return false;
		}
		void ParticleSystem::SetRelative(bool relative)
		{
		}
		float ParticleSystem::GetFading() const
		{
			return 0.0f;
		}
		void ParticleSystem::SetFading(float fading)
		{

		}

		ParticleSystem::EmitterShape ParticleSystem::GetShape() const
		{
			return EmitterShape();
		}
		void ParticleSystem::SetShape(EmitterShape shape)
		{
		}
		SharedPtr<ShaderStorageBuffer>& ParticleSystem::GetSSBO()
		{
			return m_SSBO;
		}

		Vector3 GetRandomVector3(ParticleSystem::EmitterShape shape)
		{
			Vector3 result;
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
				float linearSpeed = this->GetSpeed() == 0.0f ? 0.0001f : this->GetSpeed();

				particle.AngularParams = Matrix3(particleMatrix) * Vector3(0.0f, 1.0f, 0.0f) * m_particleAngularSpeed;

				particle.SpawnDistance = Random32::Range.GetRandom(this->GetMinSpawnDistance(), this->GetMaxSpawnDistance());
				particle.Position = particle.SpawnDistance * shapeBasedRandom + Vector3(particleMatrix[3]);
				particle.Velocity = linearSpeed * shapeBasedRandom;
				particle.Lifetime = Random32::Range.GetRandom(0.0f,1.0f) * this->GetMaxInitialTimeAlive();
				particle.Size = Random32::Range.GetRandom(this->GetMinSize(), this->GetMaxSize());
			
			}
		}
	}
}