#include "AudioManager.h"
#include <fmod_errors.h>
#include "Engine/Utils/Logging/Log.h"
#include "AudioSource.h"
#include "AudioListener.h"

#include "Engine/Core/Components/Transform.h"

namespace SaltnPepperEngine
{

	namespace Audio
	{
		FMOD::System* AudioManager::m_audiosystem = nullptr;


		FMOD_VECTOR GetFmodVector(const Vector3& glmVector)
		{
			FMOD_VECTOR vector;

			vector.x = glmVector.x;
			vector.y = glmVector.y;
			vector.z = glmVector.z;

			return vector;
		}

		Vector3 GetGlmVector(const FMOD_VECTOR& fmodVector)
		{
			return Vector3(fmodVector.x, fmodVector.y, fmodVector.z);
		}


		std::string ErrorCheckFMod(FMOD_RESULT result, int line)
		{
			// There is some Actual FMOD API error which is Real Bad
			if (result != FMOD_OK)
			{

				std::string errorMsg = "FMOD ERROR: AudioManager [Line : " + std::to_string(line) + "] " + std::to_string(result) + " - " + FMOD_ErrorString(result);
				return errorMsg;
			}

			return "";
		}


		AudioManager::AudioManager()
		{

		}


		AudioManager::~AudioManager()
		{
		}

		void AudioManager::Init()
		{

			CHECKFMODERR(FMOD::System_Create(&GetInstance().m_audiosystem));
			CHECKFMODERR(GetInstance().m_audiosystem->init(MAX_AUDIO_CHANNELS, FMOD_INIT_NORMAL, nullptr));


		}


		void AudioManager::Update(const float deltaTime)
		{
			if (!GetInstance().m_audiosystem)
			{
				LOG_ERROR("FMOD: No Audio System");
				return;
			}

			ComponentView listenerView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<AudioListenerComponent>();

			if (listenerView.Size() > 1)
			{
				LOG_WARN("FMOD : More Than One Audio Listeners in the Scene");
			}

			if (!listenerView.IsEmpty())
			{
				AudioListenerComponent& listener = listenerView[0].GetComponent<AudioListenerComponent>();
				Transform& listenerTransform = listenerView[0].GetComponent<Transform>();

				listener.GetListener()->Update(listenerTransform, deltaTime);
			}


			// Update The Sounds
			/*if (!GetInstance().m_AudioSources.empty())
			{
				for (AudioSource* source : GetInstance().m_AudioSources)
				{
					source->Update(deltaTime);
				}
			}*/

			ComponentView sourceView = Application::GetCurrent().GetCurrentScene()->GetEntityManager()->GetComponentsOfType<AudioSourceComponent>();

			if (!sourceView.IsEmpty())
			{
				for (Entity sourceEntity : sourceView)
				{
					std::string name = sourceEntity.GetComponent<NameComponent>().name;

					AudioSource* source = sourceEntity.GetComponent<AudioSourceComponent>().GetSource();
					Transform& sourcetransform = sourceEntity.GetComponent<Transform>();

					source->Update(sourcetransform, deltaTime);
				}
			}

			CHECKFMODERR(GetInstance().m_audiosystem->update());

		}

		void AudioManager::Shutdown()
		{
			if (!GetInstance().m_audiosystem)
			{
				LOG_ERROR("FMOD: No Audio System");
				return;
			}
			// Closing the system
			CHECKFMODERR(GetInstance().m_audiosystem->close());
			// Finally releaseing the system
			CHECKFMODERR(GetInstance().m_audiosystem->release());


		}

		int AudioManager::GetNewChannelIndex()
		{
			// for now 
			int newChannelIndex = m_channelMap.size();

			// Create a new Channel 
			SharedPtr<Channel> newChannel = MakeShared<Channel>();

			// Adding the created channel to the map
			m_channelMap[newChannelIndex] = newChannel;


			return newChannelIndex;
		}



		bool AudioManager::LoadSound(const AudioClip& clip, SoundType mode, LoadType type)
		{
			// initial Check t osee if the Sound clip file is already loaded
			if (clip.hasLoaded)
			{
				LOG_WARN("AudioManager: {0} already Loaded", clip.name);
				return false;
			}


			typename SoundMap::iterator itr = m_audioClips.find(clip.name);

			// Secondary check to see if the same file is not load from differnt sources
			if (itr != m_audioClips.end())
			{
				LOG_WARN("AudioManager: {0} already Exists", clip.name);
				return false;
			}

			// Creating a new sound pointer to load the sound usingthe Api
			FMOD::Sound* newSound = nullptr;

			FMOD_MODE audioMode = ((mode == SoundType::Sound2D) ? (FMOD_DEFAULT) : (FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF));

			switch (type)
			{
			case Audio::AudioManager::LoadType::SAMPLED:



				// Create a new sound using FMOD craete
				CHECKFMODERR(m_audiosystem->createSound(clip.filePath.c_str(), audioMode, nullptr, &newSound));


				break;

			case Audio::AudioManager::LoadType::STREAMED:

				// Create a new sound using FMOD craete
				CHECKFMODERR(m_audiosystem->createStream(clip.filePath.c_str(), audioMode, nullptr, &newSound));

				break;

			}





			// if the new sound is valid, we add it to the cache
			if (!newSound)
			{
				LOG_ERROR("AudioManager: Sound Returned Null {0}", clip.name);
				return false;
			}

			m_audioClips[clip.name] = newSound;


			// Updating the data in the audio clip to show that the clip ha successfully loaded
			clip.hasLoaded = true;


			LOG_INFO("AudioManager: {0} loaded Successfully", clip.name);

			return true;
		}


		void AudioManager::UnloadSound(const AudioClip& clip)
		{

			typename SoundMap::iterator itr = m_audioClips.find(clip.name);

			if (itr == m_audioClips.end())
			{
				// no sound found in the cache with the given name 
				//std::cout << "Audio Manager : " << clip.GetID() << "  : Unload Failed : Sound wasnt loaded " << std::endl;
				return;
			}

			// Release the Bound Sound
			CHECKFMODERR(itr->second->release());

			// Remove the sound from the cache
			m_audioClips.erase(itr);

		}


		int AudioManager::PlaySound(const AudioClip& clip, const int channelId)
		{
			// Iterator for finding the loaded sound from clip list
			typename SoundMap::iterator itr = m_audioClips.find(clip.name);

			// If the given sound is not found 
			if (itr == m_audioClips.end())
			{
				LOG_ERROR("Sound doesnt exist : {0}", clip.name);
				return channelId;
			}




			// Get a refence to the channel we are about to use
			SharedPtr<Channel> selectedChannel = m_channelMap[channelId];

			// Setting Looping Bit Flag
			itr->second->setMode(clip.m_shouldLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

			// Set up the playback but pause the sound initially
			//CHECKFMODERR(m_audiosystem->playSound(itr->second, 0, true, &channelRef));
			CHECKFMODERR(GetInstance().m_audiosystem->playSound(itr->second, 0, true, &selectedChannel->fmodCh));

			// Start the play back
			CHECKFMODERR(selectedChannel->fmodCh->setPaused(false));


			return channelId;
		}

		bool AudioManager::GetChannelPlaying(const int id)
		{
			if (id > m_channelMap.size()) { return false; }

			SharedPtr<Channel>& selectedChannel = m_channelMap[id];

			selectedChannel->fmodCh->isPlaying(&selectedChannel->isPlaying);

			return selectedChannel->isPlaying;

		}

		void AudioManager::StopChannelPlayback(const int id)
		{
			if (id > m_channelMap.size()) { return ; }

			SharedPtr<Channel>& selectedChannel = m_channelMap[id];

			selectedChannel->fmodCh->stop();

		}

		const bool AudioManager::GetChannelPaused(const int id) const
		{
			return false;
		}

		const unsigned int AudioManager::GetSoundCliplength() const
		{
			return 0;
		}
		const float AudioManager::GetChannelVolume(const int id) const
		{
			return GetInstance().m_channelMap[id]->volume;
		}
		const float AudioManager::GetChannelPitch(const int id) const
		{
			return 0.0f;
		}
		const float AudioManager::GetChannelPan(const int id) const
		{
			return 0.0f;
		}
		const unsigned int AudioManager::GetPlaybackPosition(const int id) const
		{
			return 0;
		}

		void AudioManager::SetChannelVolume(const float value, const int id)
		{
			m_channelMap[id]->volume = value;
			m_channelMap[id]->fmodCh->setVolume(value);
		}

		void AudioManager::SetChannelPitch(const float value, const int id)
		{

		}
		void AudioManager::SetChannelPan(const float value, const int id)
		{

		}

		void AudioManager::AddDSPFilter(int channelId, DSPEffects filter)
		{
			int chainIndex = 0;
			GetInstance().m_channelMap[channelId]->fmodCh->getNumDSPs(&chainIndex);



			switch (filter)
			{
			case Audio::REVERB:

				CHECKFMODERR(GetInstance().m_audiosystem->createDSPByType((FMOD_DSP_TYPE)filter, &m_DSP_Reverb));

				GetInstance().m_channelMap[channelId]->fmodCh->addDSP(chainIndex - 1, m_DSP_Reverb);

				break;
			case Audio::HIGHPASS:

				CHECKFMODERR(GetInstance().m_audiosystem->createDSPByType((FMOD_DSP_TYPE)filter, &m_DSP_HighPass));
				CHECKFMODERR(m_DSP_HighPass->setParameterFloat(FMOD_DSP_HIGHPASS_CUTOFF, 500.0f));
				GetInstance().m_channelMap[channelId]->fmodCh->addDSP(chainIndex - 1, m_DSP_HighPass);

				break;
			case Audio::LOWPASS:

				CHECKFMODERR(GetInstance().m_audiosystem->createDSPByType((FMOD_DSP_TYPE)filter, &m_DSP_LowPass));
				CHECKFMODERR(m_DSP_LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, 1000.0f));
				GetInstance().m_channelMap[channelId]->fmodCh->addDSP(chainIndex - 1, m_DSP_LowPass);
				break;
			case Audio::DISTORTION:

				CHECKFMODERR(GetInstance().m_audiosystem->createDSPByType((FMOD_DSP_TYPE)filter, &m_DSP_Distortion));
				CHECKFMODERR(m_DSP_Distortion->setParameterFloat(FMOD_DSP_DISTORTION_LEVEL, 0.7f));
				GetInstance().m_channelMap[channelId]->fmodCh->addDSP(chainIndex - 1, m_DSP_Distortion);

				break;

			case Audio::ECHO:

				CHECKFMODERR(GetInstance().m_audiosystem->createDSPByType((FMOD_DSP_TYPE)filter, &m_DSP_Echo));
				CHECKFMODERR(m_DSP_Echo->setParameterFloat(FMOD_DSP_ECHO_DELAY, 500.0f));
				CHECKFMODERR(m_DSP_Echo->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 60.0f));
				GetInstance().m_channelMap[channelId]->fmodCh->addDSP(chainIndex - 1, m_DSP_Echo);

				break;
			default:
				break;
			}


			SetDSPState(channelId, filter, false);
		}

		void AudioManager::SetDSPState(int channelId, DSPEffects filter, bool isActive)
		{
			//For now since there int a lot of time the DSP effets are hardcoded,

			// Later need to have the effects on teh actual source

			switch (filter)
			{
			case Audio::REVERB:  GetInstance().m_DSP_Reverb->setBypass(!isActive);

				break;
			case Audio::HIGHPASS:  GetInstance().m_DSP_HighPass->setBypass(!isActive);
				break;
			case Audio::LOWPASS:  GetInstance().m_DSP_LowPass->setBypass(!isActive);
				break;

			case Audio::DISTORTION: GetInstance().m_DSP_Distortion->setBypass(!isActive);

				break;
			case Audio::ECHO: GetInstance().m_DSP_Echo->setBypass(!isActive);
				break;
			default:
				break;
			}
		}


		void AudioManager::SetSource3DMinMaxDist(const int channelId, const float min, const float max)
		{
			CHECKFMODERR(GetInstance().m_channelMap[channelId]->fmodCh->set3DMinMaxDistance(min, max));

			float minTest = 0;
			float maxtest = 0;

			FMOD::Channel* cha = GetInstance().m_channelMap[channelId]->fmodCh;

			CHECKFMODERR(GetInstance().m_channelMap[channelId]->fmodCh->get3DMinMaxDistance(&minTest, &maxtest));

			LOG_WARN("{0} || {1}", std::to_string(minTest), std::to_string(maxtest));


			GetInstance().m_channelMap[channelId]->fmodCh->set3DDopplerLevel(5.0f);
		}

		void AudioManager::SetSource3DAttributes(const int channelId, const Vector3& position, const Vector3& velocity)
		{
			FMOD_VECTOR pos = GetFmodVector(position);
			FMOD_VECTOR vel = GetFmodVector(velocity);
			m_channelMap;
			CHECKFMODERR(GetInstance().m_channelMap[channelId]->fmodCh->set3DAttributes(&pos, &vel));
		}

		void AudioManager::SetListenerAttributes(const Vector3& position, const Vector3& velocity, const Vector3& forward, const Vector3& up)
		{
			FMOD_VECTOR fModPosition = GetFmodVector(position);
			FMOD_VECTOR fModVelocity = GetFmodVector(velocity);
			FMOD_VECTOR fModForward = GetFmodVector(-forward);
			FMOD_VECTOR fModUp = GetFmodVector(up);

			CHECKFMODERR(GetInstance().m_audiosystem->set3DListenerAttributes(0, &fModPosition, &fModVelocity, &fModForward, &fModUp));

			//CHECKFMODERR(m_audiosystem->getGeometryOcclusion(&fModPosition, &origin, &direct, &reverb));

		}

		void AudioManager::AddOcclusionPolygon(AudioGeometry* geo, const Transform* transform, const Vector2& dim)
		{
			int index = 0;

			// For now for simplicity lets just have a quad as the occluding polygon
			const int numberVertices = 4;

			const float halfSizeX = dim.x / 2;
			const float halfSizeY = dim.y / 2;


			FMOD_VECTOR quad[numberVertices] =
			{
				{halfSizeX, halfSizeY, 0.0f}, // 
				{halfSizeX,-halfSizeY,0.0f},
				{-halfSizeX, -halfSizeY,0.0f},
				{-halfSizeX, halfSizeY,0.0f}
			};

			CHECKFMODERR(geo->geometry->addPolygon(0.9f, 0.9f, true, 4, quad, &geo->polyIndex));

			Vector3 forward = transform->GetRotation() * Vector3(0.0f, 0.0f, 1.0f);
			forward.y = 0;

			FMOD_VECTOR geoposition = GetFmodVector(transform->GetPosition());
			FMOD_VECTOR geoForward = GetFmodVector(forward);
			FMOD_VECTOR geoUp = GetFmodVector(Vector3(0.0f, 1.0f, 0.0f));


			geo->geometry->setPosition(&geoposition);
			geo->geometry->setRotation(&geoForward, &geoUp);


		}

		AudioGeometry* AudioManager::CreateGeometry(Entity& entity)
		{

			AudioGeometry* audioGeo = &entity.AddComponent<AudioGeometry>();

			CHECKFMODERR(GetInstance().m_audiosystem->createGeometry(2, 6, &audioGeo->geometry));


			GetInstance().m_GeometryList.push_back(audioGeo);


			audioGeo->geometry->setActive(true);

			return audioGeo;
		}

		AudioSource* AudioManager::CreateSource(Entity& entity)
		{

			AudioSource* audioSource = entity.AddComponent<AudioSourceComponent>().GetSource();
			audioSource->RegisterSource();

			SetSource3DMinMaxDist(audioSource->GetChannelIndex(), audioSource->Get3DMinDist(), audioSource->Get3DMaxDist());

			return audioSource;
		}



		void AudioManager::GetListernerAttributes()
		{

			FMOD_VECTOR fmodPosition;
			FMOD_VECTOR fmodVelocity;
			FMOD_VECTOR fmodForward;
			FMOD_VECTOR fmodUp;

			CHECKFMODERR(GetInstance().m_audiosystem->get3DListenerAttributes(
				0, &fmodPosition, &fmodVelocity, &fmodForward, &fmodUp));

			Vector3 position = GetGlmVector(fmodPosition);
			Vector3 forward = GetGlmVector(fmodForward);

			//LOG_WARN(" Position {0} : {1} : {2}", position.x, position.y, position.z);
			LOG_WARN(" forward {0} : {1} : {2}", forward.x, forward.y, forward.z);

		}


	}
}