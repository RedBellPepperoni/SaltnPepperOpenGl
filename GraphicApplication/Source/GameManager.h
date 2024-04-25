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
		void UpdateEnemies(const float deltaTime, const Vector3& playerPos, const Vector3& buddyPos);
		void UpdateBuddy(const float deltaTime);

		void OnInitTestScene();
		void OnInitMainScene();

		void SetupNavMesh();
		void SetupPathFinder();

		void SetupStaticMeshes();
		void SetupStaticPhysics();


		void OnUpdateTestScene(const float deltaTime);
		void OnUpdateMainScene(const float deltaTime);

		void UpdateMarkerAnim(const float deltaTime);


		void DebugNavmesh();

	public:

		GameManager() = default;
		~GameManager() = default;

		void OnInit();
		void OnUpdate(const float deltaTime);

		void MoveBuddyTo(const Vector3& position,RigidBody* markedEnemy = nullptr);
		void CheckandUpdateMarkedEnemy(const float& deltatime);
		void HideMarker();

		void SetEnemyDeath(RigidBody* enemyRef);

		void ResetLevel();

	private:


		
		Vector3 m_playerPosition = Vector3{ 0.0f };

		SharedPtr<NavMesh> m_navMesh = nullptr;
		SharedPtr<Pathfinder> m_pathFinder = nullptr;

		std::vector<Vector3> m_path;
		std::vector<Vector3> m_simplifiedpath;
		Vector3 m_buddyPosition;

		PlayerCharacter* m_playerRef = nullptr;
		BuddyCharacter* m_buddyRef = nullptr;

		std::vector<NavTriangle> m_debugnavmesh;
		Entity m_waypointbaseEntity;
		Entity m_waypointarrowEntity;

		
		RigidBody* m_markedEnemy = nullptr;
		float sincounter = 0.0f;
		float rotcounter = 0.0f;

		float markenemycounter;
		const float m_markRefresh = 1.5f;

		std::vector<Vector3> m_zombieSpawns;
		Vector3 m_buddySpawn;
		Vector3 m_buddyLook;
		Vector3 m_playerSpawn;
		Vector3 m_playerLook;
		
	};
}


#endif // !GAMEMANAGER_H
