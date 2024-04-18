#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include "SaltnPepperEngine.h"
#include "IDamagable.h"

namespace SaltnPepperEngine
{
	class GameManager;

	struct PlayerLook
	{
		std::string look_tag = "PlayerLook";
	};

	struct EnvironmentTag
	{
		enum class Tag : uint8_t
		{
			NONE,
			ENEMY,
			BUDDY,
			NAVMESH,
			LIGHT,
			TV
		};

		Tag currentTag = Tag::NONE;
	};


	class PlayerCharacter : public IDamagable
	{
	private:

		enum class GunAudio : uint8_t
		{
			SHOOT,
			RELOAD,
			EMPTY
		};


	private:



		void KeyBoardInput(const float deltaTime, Transform& lookTransform);
		void MouseInput(const float deltaTime, Vector2 mousePosition, Transform& lookTransform);

		
		bool RayCastbyTag(const Vector3& origin, const Vector3& forward,  bool navcast = false);

		void PlayGunAudio(const GunAudio& audiotype);
		
		void Shoot(const Transform& look);
		
	public:

		PlayerCharacter() = default;
		virtual ~PlayerCharacter() = default;

		void SetAnimatorRef(SkinnedAnimator* animRef);
		SkinnedAnimator* GetAnimator();
		void SetRigidBodyRef(RigidBody* bodyRef);
		void SetGameManagerRef(GameManager* finder);

		void OnUpdate(float deltaTime, Vector2 mousePosition, Transform& lookTransform);

		virtual void TakeDamage(const int damage, const DamageSource& source) override;

		void SetAudioSources(Audio::AudioSource* footstep, Audio::AudioSource*gun);
		void SetAudioClipsGun(AudioClip* shoot, AudioClip* empty, AudioClip* reload);
	

	private:

		int m_playerCameraIndex = 0;
		float m_speed = 2.0f;

		const float m_forceMultiplier = 1.0f;

		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;

		float m_sensitivity = 0.00001f;
		Vector2 m_rotationalVelocity = Vector2{0.0f};
		Vector2 m_previousCursorPosition = Vector2{ 0.0f };
		float m_rotationalDampening = 0.00005f;;


		Audio::AudioSource* m_footStepSource = nullptr;
		Audio::AudioSource* m_gunSource = nullptr;

		Audio::AudioListener* m_listener = nullptr;

		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

		float m_shootCooldown = 0.12f;
		float m_shootcounter = 0.0f;

		float m_ReloadTimer = 3.6f;
		float m_reloadCounter = 0.0f;

		bool isReloading = false;

		SharedPtr<SkinnedAnimation> idleAnim;
		SharedPtr<SkinnedAnimation> shootAnim;
		SharedPtr<SkinnedAnimation> reloadAnim;

	
		GameManager* m_gameManagerRef = nullptr;


		AudioClip* m_gunShootClip = nullptr;
		AudioClip* m_gunEmptyClip= nullptr;
		AudioClip* m_gunReloadClip = nullptr;

		int m_magzineBullets = 12;
		const int m_magzineSize = 12;
		const int m_maxBullets = 60;
		int m_totalBullets = 60;

	};


	struct PlayerComponent
	{
		PlayerComponent();
		~PlayerComponent();

		PlayerCharacter* GetPlayer();
	private:

		SharedPtr<PlayerCharacter> m_playerhandle = nullptr;
	};


}

#endif // !PLAYERCHARACTER_H

