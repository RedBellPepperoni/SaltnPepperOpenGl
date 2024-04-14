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

		void SetupNavMesh();
		void SetupPathFinder();

		void SetupStaticMeshes();
		void SetupStaticPhysics();


		void OnUpdateTestScene(const float deltaTime);
		void OnUpdateMainScene(const float deltaTime);


		void DebugNavmesh();

	public:

		GameManager() = default;
		~GameManager() = default;

		void OnInit();
		void OnUpdate(const float deltaTime);



	private:


		RigidBody* body = nullptr;
		Vector3 playerPosition = Vector3{ 0.0f };

		SharedPtr<NavMesh> m_navMesh = nullptr;
		SharedPtr<Pathfinder> m_pathFinder = nullptr;

		std::vector<Vector3> triCenters;
		std::vector<Vector3> path;
		std::vector<Vector3> simplifiedpath;

		PlayerCharacter* m_playerRef = nullptr;


	};
}


#endif // !GAMEMANAGER_H
