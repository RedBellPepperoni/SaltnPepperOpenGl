#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H

#include "SaltnPepperEngine.h"
#include "IDamagable.h"

namespace SaltnPepperEngine
{
	struct PlayerLook
	{
		std::string look_tag = "PlayerLook";
	};


	class PlayerCharacter : public IDamagable
	{

	private:

		void KeyBoardInput(const float deltaTime, Transform& lookTransform);
		void MouseInput(const float deltaTime, Vector2 mousePosition, Transform& lookTransform);


	public:

		PlayerCharacter() = default;
		virtual ~PlayerCharacter() = default;

		void SetAnimatorRef(SkinnedAnimator* animRef);
		void SetRigidBodyRef(RigidBody* bodyRef);

		void OnUpdate(float deltaTime, Vector2 mousePosition, Transform& lookTransform);

		virtual void TakeDamage(const int damage) override;




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


		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;

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

