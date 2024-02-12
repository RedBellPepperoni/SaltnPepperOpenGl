#ifndef PLAYERCHARACTER_H
#define PLAYERCHARACTER_H





namespace SaltnPepperEngine
{
	namespace Components
	{
		class Transform;
	}

	namespace Physics
	{
		class RigidBody3D;
	}

	using namespace Physics;
	using namespace Components;

	class PlayerCharacter
	{
	public:

		PlayerCharacter();
		~PlayerCharacter();

		void Init(RigidBody3D* RigidBodyRef, Transform* lookTransformRef, Transform* cameraTransformRef);
		void Update(float deltaTime);
		void ProcessKeyboardInput(Transform& cameratransform, float deltaTime);

	private:

	
		void ProcessMouseInput();

	private:

		RigidBody3D* m_rigidBodyRef = nullptr;
		Transform* m_lookTransfrom = nullptr;
		Transform* m_cameraRef = nullptr;
		int m_health = 100;
		float m_moveSpeed = 2.0f;
	};


}

#endif // !PLAYERCHARACTER_H

