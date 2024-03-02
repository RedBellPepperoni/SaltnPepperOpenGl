#ifndef TREASUREHUNTER_H
#define TREASUREHUNTER_H
#include "AstarFinder.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include "DungeonManager.h"
#include "Engine/Core/Components/Transform.h"
#include "Graph.h"



namespace SaltnPepperEngine
{

	
	enum class HunterState
	{
		IDLE,
		MOVING,
		SEARCHING,
		MAX
	};
	using namespace Components;

	class TreasureHunter
	{
	public:
		TreasureHunter();
		~TreasureHunter();

		void SetGraph(const SharedPtr<Graph>& graph, SharedPtr<DungeonManager>& man);
		void SetSpawn(const int x, const int y);
		void SetTransform(Transform* transform);

		void SetNewTarget(const int x, const int y);

		void Update(const float deltaTime);

		const Vector2 GetPosition() const;

		void ReachedTarget(bool collected);
		void SetTimeToMove(float time) { timeToMove = time; }

	private:

		int GetIndex(int x, int y);

		Vector2Int currentTargetPos;

		UniquePtr<AstarFinder> pathFinder = nullptr;
		
		Transform* hunterTransform;
		SharedPtr<Graph> graphCopy = nullptr;
		SharedPtr<Graph> graphBase = nullptr;
		SharedPtr<DungeonManager> manager = nullptr;

		std::vector<Vector3> currentPath;
		int treasureCount = 0;
		HunterState currentState = HunterState::IDLE;

		int currentPathNode = 0;

		float counter = 0.0f;
		// time the hunter moves 1 unit in seconds
		float timeToMove = 0.1f;
	};


	

}

#endif // !TREASUREHUNTER_H
