#pragma once
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	using Components::Transform;

	

	namespace Audio
	{
		struct AudioClip;
		class AudioManager;
		enum DSPEffects : uint8_t;

		class AudioSource
		{
		public:

			AudioSource();
			~AudioSource();

			bool SetAudioClip(AudioClip* clip);

			AudioClip* GetClip();

			void PlayClip();

			void Update(const Transform& transform, float deltaTime);

			void RegisterSource();

			const float Get3DMinDist() const { return m_min3DDistance; }
			const float Get3DMaxDist() const { return m_max3DDistance; }

			void Set3DMinDist(const float newMin);
			void Set3DMaxDist(const float newMax);

			const int GetChannelIndex() const { return m_channelIndex; }

			void SetLoop(const bool loop);
			const bool GetLoop() const { return m_shouldLoop; }

			void AddDSPEffect(DSPEffects filter);

			void SetDSPState(DSPEffects filter, const bool isactive);

		protected:

			Vector3 CalculateVelocity(const Transform& transform,const float deltatime);

		protected:


			AudioClip* m_audioClip = nullptr;

			bool m_shouldLoop = false;

			bool m_is3D = true;

			bool m_hasClip = false;

			int m_channelIndex = -1;


			float m_volume = 1.0f;

			float m_rollOff = 1.0f;

			static int sourceCount;

			Vector3 lastPosition = Vector3(0.0f);


			float m_min3DDistance = 0.5f;
			float m_max3DDistance = 500.0f;

		};

		
	}
}

