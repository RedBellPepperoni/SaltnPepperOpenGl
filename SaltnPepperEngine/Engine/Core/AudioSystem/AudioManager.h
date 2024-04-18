#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <fmod.hpp>
#include <map>
#include <string>
#include <vector>
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "Engine/Utils/Singleton.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include "Engine/Core/EntitySystem/EntityManager.h"


namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	using Components::Transform;

	namespace Audio
	{
		class AudioSource;


		struct AudioGeometry
		{

			FMOD::Geometry* geometry = nullptr;

			int polyIndex;

		};


		struct AudioClip
		{

			AudioClip(const std::string& newName, const std::string& newfilePath)
			{
				name = newName;
				filePath = newfilePath;
			}

			// For now the uniqueID is the disk path for the sound
			std::string name = "unknown";

			std::string filePath = "default";

			// Stores if the Sound has been loaded into the Engine
			mutable bool hasLoaded = false;

			// Store if the Sound is OneShot or looping
			bool m_shouldLoop = false;


		};

		struct Channel
		{
			// the name of the channel
			std::string name = "DefaultChannel";
			// The pointer to the actual fmod channel
			FMOD::Channel* fmodCh = nullptr;

			float volume = 1.0f;

			float pitch = 1.0f;

			float pan = 0.0f;

			bool isPlaying = false;

		};

		static std::string ErrorCheckFMod(FMOD_RESULT result, int line);


#define CHECKFMODERR(fModResult) ErrorCheckFMod(fModResult,__LINE__); 


#define CHGROUP_MASTER	0
#define CHGROUP_MUSIC	1
#define CHGROUP_SFX		2


		enum DSPEffects : uint8_t
		{
			REVERB = FMOD_DSP_TYPE_SFXREVERB,
			HIGHPASS = FMOD_DSP_TYPE_HIGHPASS,
			LOWPASS = FMOD_DSP_TYPE_LOWPASS,
			DISTORTION = FMOD_DSP_TYPE_DISTORTION,
			ECHO = FMOD_DSP_TYPE_ECHO
		};


		class AudioManager : public Singleton<AudioManager>
		{

		public:

			enum class LoadType
			{
				SAMPLED,
				STREAMED
			};

			enum class SoundType
			{
				Sound2D,
				Sound3D
			};


			// Type Definitions for readability
			typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;
			typedef std::unordered_map<int, SharedPtr<Channel>> ChannelMap;

			// Constructor
			AudioManager();

			//Destructor
			virtual ~AudioManager();

			// Initialized the Core Audio Systems
			static void Init();

			// Called every Frame of the Gameloop
			static void Update(const float deltaTime);

			// Called to Deactivate/ Shutdown the System after use
			static void Shutdown();


			int GetNewChannelIndex();


			// Sound Loading / Unloading
			bool LoadSound(const AudioClip& clip, SoundType mode = SoundType::Sound2D, LoadType type = LoadType::SAMPLED);


			void UnloadSound(const AudioClip& clip);

			// Sound PlayBack
			int PlaySound(const AudioClip& clip, const int channelId);

			bool GetChannelPlaying(const int id = 0);
			void SetChannelLoop(const int id = 0,bool loop = false);

			void StopChannelPlayback(const int id = 0);

			// Getter for checking if the current channel is paused
			const bool GetChannelPaused(const int id = 0) const;

			// The Current playing sound's total legnth
			const unsigned int GetSoundCliplength() const;

			const float GetChannelVolume(const int id = 0) const;
			const float GetChannelPitch(const int id = 0) const;
			const float GetChannelPan(const int id = 0) const;

			// The position of the audio playback
			const unsigned int GetPlaybackPosition(const int id = 0) const;

			// Modifying Channel Values
			void SetChannelVolume(const float value, const int id = 0);
			void SetChannelPitch(const float value, const int id = 0);
			void SetChannelPan(const float value, const int id = 0);


			void AddDSPFilter(int channelId, DSPEffects filter);
			void SetDSPState(int channelId, DSPEffects filter, bool isActive);


			void SetSource3DMinMaxDist(const int channelId, const float min, const float max);
			void SetSource3DAttributes(const int channelId, const Vector3& position, const Vector3& velocity);

			void SetListenerAttributes(const Vector3& position, const Vector3& velocity, const Vector3& forward, const Vector3& up);


			void AddOcclusionPolygon(AudioGeometry* geo, const Transform* transform, const Vector2& dim);


			AudioGeometry* CreateGeometry(Entity& entity);

			AudioSource* CreateSource(Entity& entity,const bool is3D = true);


			void GetListernerAttributes();


		private:

			inline FMOD::ChannelGroup* GetMasterChannelGroup()
			{
				if (m_channelGroupList.size() < CHGROUP_MASTER)
				{
					LOG_ERROR("Master Channel Group doesn't exist");
					return nullptr;
				}

				return m_channelGroupList[CHGROUP_MASTER];
			}



		private:

			// The maximum number of Channels for the audio engine 
			static const uint32_t MAX_AUDIO_CHANNELS = 16;


			// pointer to the audiosubsysem from the Fmod api
			static FMOD::System* m_audiosystem;

			int m_nextChannelId = 0;

			// The current seek position (how many miliseconds we are into the playback)
			unsigned int m_CurrentClipLength = 0;



			// The list of all the currently loaded (playable sounds)
			SoundMap m_audioClips;

			// Extera storage for channels if multiple audio playback is needed
			// Here the first channel (m_channel[0]) is always reserved for one shot sounds;
			ChannelMap m_channelMap;


			std::vector<AudioGeometry*> m_GeometryList;


			std::vector<FMOD::ChannelGroup*> m_channelGroupList;


			std::vector<AudioSource*> m_AudioSources;


			FMOD::DSP* m_DSP_Reverb;
			FMOD::DSP* m_DSP_LowPass;
			FMOD::DSP* m_DSP_HighPass;
			FMOD::DSP* m_DSP_Echo;
			FMOD::DSP* m_DSP_Distortion;

		};
	}
}

#endif // !AUDIOMANAGER_H
