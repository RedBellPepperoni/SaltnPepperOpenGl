#include "AudioSource.h"
#include "AudioManager.h"
#include "Engine/Core/Components/Transform.h"


namespace SaltnPepperEngine
{
	using namespace Components;


	namespace Audio
	{
		AudioSource::AudioSource()
		{
			
		}

		AudioSource::~AudioSource()
		{

		}

		bool AudioSource::SetAudioClip(AudioClip* clip)
		{
			m_audioClip = clip;

			AudioManager::GetInstance().LoadSound(*clip, AudioManager::SoundType::Sound3D);
			


			return false;
		}

		AudioClip* AudioSource::GetClip()
		{
			return m_audioClip;
		}


		void AudioSource::PlayClip()
		{
			if (m_channelIndex < 0)
			{
				LOG_ERROR("AudioSource : Channel not Initialized for Clip {0}", m_audioClip->name);
				return;
			}

			AudioManager::GetInstance().PlaySound(*GetClip(), m_channelIndex);
			//AudioManager::GetInstance().SetSource3DAttributes(m_channelIndex ,,Vector3(0.0f));

		}



		void AudioSource::Update(const Transform& transform, float deltaTime)
		{
			

			Vector3 position = transform.GetPosition();
			CalculateVelocity(transform, deltaTime);
			AudioManager::GetInstance().SetSource3DAttributes(m_channelIndex, position, Vector3(0.0f));

			
		}


		void AudioSource::RegisterSource()
		{
			// Register this source and add it to the audio manager's current list of active sources
			m_channelIndex = AudioManager::GetInstance().GetNewChannelIndex();

			LOG_WARN("Channel Index : {0}", std::to_string(m_channelIndex));

		}

		void AudioSource::Set3DMinDist(const float newMin)
		{
			m_min3DDistance = newMin;
			AudioManager::GetInstance().SetSource3DMinMaxDist(m_channelIndex,newMin, m_max3DDistance);

		}

		void AudioSource::Set3DMaxDist(const float newMax)
		{
			m_max3DDistance = newMax;
			AudioManager::GetInstance().SetSource3DMinMaxDist(m_channelIndex, m_min3DDistance, newMax);
		}


		void AudioSource::SetLoop(const bool loop)
		{
			m_shouldLoop = loop;
			m_audioClip->m_shouldLoop = loop;
		}

		void AudioSource::AddDSPEffect(DSPEffects filter)
		{
			AudioManager::GetInstance().AddDSPFilter(m_channelIndex, filter);
		}

		void AudioSource::SetDSPState(DSPEffects filter, const bool isactive)
		{
			AudioManager::GetInstance().SetDSPState(m_channelIndex, filter, isactive);
		}

		Vector3 AudioSource::CalculateVelocity(const Transform& transform ,const float deltatime)
		{
			Vector3 currentPosition = transform.GetPosition();

			float distance = Distance(currentPosition, lastPosition);

			lastPosition = currentPosition;

			if (distance > 0.01f)
			{

				float speed = distance / deltatime;

				Vector3 direction = Normalize(lastPosition - currentPosition);
				return direction * speed;
			}


			return Vector3(0.0f);
		}
	}
}