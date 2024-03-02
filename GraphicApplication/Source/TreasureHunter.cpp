#include "TreasureHunter.h"
#include "GraphNode.h"

namespace SaltnPepperEngine
{
	TreasureHunter::TreasureHunter()
	{
		pathFinder = MakeUnique<AstarFinder>();
	}

	TreasureHunter::~TreasureHunter()
	{
	}

	void TreasureHunter::SetGraph(const SharedPtr<Graph>& graph, SharedPtr<DungeonManager>& man)
	{
		manager = man;

		auto euclideanDistance = [](GraphNode* pNode, Graph* pGraph)
			{
				auto pEnd = pGraph->GetEndNode();
				return Length(pEnd->GetPosition() - pNode->GetPosition());
			};

		graphBase = MakeShared<Graph>();


		graphBase->Create(manager->GetWallData(), 115, 149, 1.0f, 1.0f);
		graphBase->SetHeuristicFunction(euclideanDistance);
	
		
	}

	void TreasureHunter::SetSpawn(const int x, const int y)
	{
		Vector3 spawnPos = Vector3(x, 0.0f, y);
		hunterTransform->SetPosition(spawnPos);

	}

	void TreasureHunter::SetTransform(Transform* transform)
	{
		hunterTransform = transform;
	}

	void TreasureHunter::SetNewTarget(const int x, const int y)
	{
		currentPath.clear();

		currentState = HunterState::SEARCHING;

		int currentX = hunterTransform->GetPosition().x;
		int currentY = hunterTransform->GetPosition().z;

		graphCopy = MakeShared<Graph>(*graphBase);

		pathFinder->SetBegin(GetIndex(currentX, currentY));
		pathFinder->SetEnd(GetIndex(x, y));
		pathFinder->FindPath(graphCopy);


		std::deque<GraphNode*> path = pathFinder->GetPath();

		if (path.empty())
		{
			// Shouldnt really happen but still an edge catch
			return;
		}

		for (GraphNode* node : path)
		{
			Vector3 position = Vector3(node->GetPosition().x, 0.0f, node->GetPosition().y);

			currentPath.push_back(position);
		}


		currentState = HunterState::MOVING;


	}

	void TreasureHunter::Update(const float deltaTime)
	{
		counter += deltaTime;

		if (counter < timeToMove)
		{
			return;
		}

		counter = 0.0f;


		switch (currentState)
		{
		
		case SaltnPepperEngine::HunterState::MOVING:

			// Found a treasure , take a break
			if (currentPathNode >= currentPath.size()) { currentState = HunterState::IDLE; 
			return;
			}

			Vector3 nextPosition = currentPath[currentPathNode];

			hunterTransform->SetPosition(nextPosition);

			currentPathNode++;


			break;

		default:
			break;
		}


	}

	const Vector2 TreasureHunter::GetPosition() const
	{
		Vector2 position = Vector2(hunterTransform->GetPosition().x, hunterTransform->GetPosition().z);

		return position;
	}



	

	void TreasureHunter::ReachedTarget(bool collected)
	{
		if (collected)
		{
			treasureCount++;
		}


	}

	int TreasureHunter::GetIndex(int x, int y)
	{
		return x + y * 115;
	}
}