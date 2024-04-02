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
		void MouseInput(const float deltaTime, Transform& lookTransform);


	public:

		PlayerCharacter() = default;
		virtual ~PlayerCharacter() = default;

		void SetAnimatorRef(SkinnedAnimator* animRef);
		void SetRigidBodyRef(RigidBody* bodyRef);

		void OnUpdate(float deltaTime, Transform& lookTransform);

		virtual void TakeDamage(const int damage) override;

		


	private:

		int m_playerCameraIndex = 0;
		float m_speed = 10.0f;
		


		SkinnedAnimator* m_animator = nullptr;
		RigidBody* m_rigidBody = nullptr;

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

