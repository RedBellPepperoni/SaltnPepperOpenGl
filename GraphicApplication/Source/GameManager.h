#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "SaltnPepperEngine.h"
#include "EntitySetup.h"

namespace SaltnPepperEngine
{
	class GameManager
	{

	private:

		void UpdatePlayer(const float deltaTime);
		void UpdateEnemies(const float deltaTime, const Vector3& playerPos);


		void OnInitTestScene();
		void OnInitMainScene();


		void SetupStaticMeshes();
		void SetupStaticPhysics();


		void OnUpdateTestScene(const float deltaTime);
		void OnUpdateMainScene(const float deltaTime);


	public:

		GameManager() = default;
		~GameManager() = default;

		void OnInit();
		void OnUpdate(const float deltaTime);



	private:


		RigidBody* body = nullptr;
		Vector3 playerPosition = Vector3{ 0.0f };
	};
}


#endif // !GAMEMANAGER_H
