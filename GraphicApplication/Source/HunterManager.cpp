#include "HunterManager.h"
#include "EntitySetup.h"



namespace SaltnPepperEngine
{
	void HunterManager::CreateDungeon()
	{
		manager = MakeShared<DungeonManager>();

		manager->LoadTSVMap("Assets\\Data\\DungeonCleanCompact.tsv");
		manager->SetStartandEnd();

		//manager->PrintMap();
		manager->Create3DMap();

		auto euclideanDistance = [](GraphNode* pNode, Graph* pGraph)
			{
				auto pEnd = pGraph->GetEndNode();
				return Length(pEnd->GetPosition() - pNode->GetPosition());
			};

		dungeonGraph = MakeShared<Graph>();


		dungeonGraph->Create(manager->GetWallData(), 115, 149, 1.0f, 1.0f);
		dungeonGraph->SetHeuristicFunction(euclideanDistance);


	}
	void HunterManager::CreateHunters(int count)
	{
		

		for (int i = 0; i < count; i++)
		{
			SharedPtr<TreasureHunter> hunter = MakeShared<TreasureHunter>();
			Transform* T = CreateHunter(Vector3(1.0f, 0.0f, 1.0f));
			hunter->SetGraph(dungeonGraph,manager);
			hunter->SetTransform(T);
			hunter->SetSpawn(1, 1);
			hunter->SetTimeToMove(Random32::Range.GetRandom(1.0f, 0.8f));
			hunter->SetHunterIndex(i);
			hunters.push_back(hunter);

			
			SharedPtr<HunterThreadInfo> HTParams = MakeShared<HunterThreadInfo>();
			HTParams->hunterRef = hunters[i];
			
			HTParams->isAlive = true;
			HTParams->run = true;

			threadInfos.push_back(HTParams);

			HunterThreadInfo* htInfo = HTParams.get();

			DWORD ThreadId = 0;
			HANDLE threadHandle = 0;
			threadHandle = CreateThread(
				NULL,					// lpThreadAttributes,
				0,						// dwStackSize,
				UpdateThreadHunter,		// lpStartAddress,
				(void*)htInfo,				//  lpParameter,
				0,						// dwCreationFlags (0 or CREATE_SUSPENDED)
				&ThreadId); // lpThreadId


			ThreadId++;


			threadhandleList.push_back(threadHandle);
		}

	}

	void HunterManager::LaunchHunters()
	{
		for (int i = 0; i < hunters.size(); i++)
		{
			Vector2Int pos = GetRandomAvailableTreasure();
			hunters[i]->SetNewTarget(pos.x,pos.y);
		}
	}

	void HunterManager::CreateTreasures(int count)
	{
		for (int i = 0; i < count; i++)
		{
			Transform* treasturetransform = SpawnRandomTreasure();

			availableTreasures.push_back(treasturetransform);
		}
	}
	Vector3 HunterManager::GetPositionfromIndex(int index)
	{
		int y = index / 115;
		int x = index % 115;

		return Vector3(x, 0.0f, y);

	}

	int HunterManager::GetRandomFloorIndex()
	{
		int index = Random32::Range.GetRandom(0, dungeonGraph->GetNodes().size() - 1);

		if (dungeonGraph->GetNodes()[index]->IsBlocked() == false)
		{
			return index;
		}


		return GetRandomFloorIndex();
	}

	Transform* HunterManager::SpawnRandomTreasure()
	{
		int randomIndex = GetRandomFloorIndex();

		Vector3 spawnPos = GetPositionfromIndex(randomIndex);

		Transform* tr = CreateTreasure(spawnPos);

		return tr;
	}

	Vector2Int HunterManager::GetRandomAvailableTreasure()
	{
		if (availableTreasures.empty())
		{
			return Vector2Int(1, 1);
		}

		int randomIndex = Random32::Range.GetRandom(0, availableTreasures.size() - 1);
		Vector3 pos = availableTreasures[randomIndex]->GetPosition();
		Vector2Int out = Vector2Int(pos.x, pos.z);




		return out;
	}

	void HunterManager::MovetoNextTreaure(SharedPtr<TreasureHunter>& hun)
	{
		//Vector2Int pos = GetRandomAvailableTreasure();
		//if (!hun->SetNewTarget(pos.x, pos.y))
		//{
		//	//hun->
		//}
	}

	void HunterManager::UsedUpTreasure(const Vector3 position)
	{
		for (int i = 0; i < availableTreasures.size(); i++)
		{
			Vector3 treasurePos = availableTreasures[i]->GetPosition();

			bool Xequal = abs(position.x - treasurePos.x) < 0.2f;
			bool Yequal = abs(position.z - treasurePos.z) < 0.2f;

			if (Xequal && Yequal)
			{
				availableTreasures[i]->SetPosition(Vector3(0.0f, -500.0f, -0.0f));
				pickedTreasures.push_back(availableTreasures[i]);

				availableTreasures[i] = availableTreasures.back();
				availableTreasures.pop_back();

			}
		}
	}

	void HunterManager::UsedUpTreasure(const int index)
	{
		availableTreasures[index]->SetPosition(Vector3(0.0f, -500.0f, -0.0f));
		pickedTreasures.push_back(availableTreasures[index]);

		availableTreasures[index] = availableTreasures.back();
		availableTreasures.pop_back();
	}

	void HunterManager::Update(const float deltaTime)
	{
		for (SharedPtr<TreasureHunter>& hun : hunters)
		{
			/*hun->Update(deltaTime);*/

			if (hun->GetFinal()) { continue; }

			if (availableTreasures.empty())
			{

				Vector2 pos = hun->GetPosition();
				hun->SetNewTarget(1,1);
				if (abs(pos.x - 0) < 0.03f && abs(pos.y - 0) < 0.03f)
				{
					hun->SetFinal();
					int hunterIndex = hun->GetHunterIndex();
					int treasurecount = hun->GetTreasureCount();

					LOG_CRITICAL("Hunter [{0}] collected : [{1}] Treasures",hunterIndex,treasurecount);
				}
			}
			

			for (int i = 0; i < availableTreasures.size(); i++)
			{
				Vector2 pos = hun->GetPosition();
				Vector3 tPos = availableTreasures[i]->GetPosition();

				if (abs(pos.x - tPos.x) < 0.03f && abs(pos.y - tPos.z) < 0.03f)
				{


					hun->ReachedTarget(true);
					UsedUpTreasure(i);

					Vector2Int pos = GetRandomAvailableTreasure();
					/*if (!hun->SetNewTarget(pos.x, pos.y))
					{

					}*/
					int hunterIndex = hun->GetHunterIndex();

					LOG_INFO("Hunter [{0}] collected Treasure ", hunterIndex);
					

					hun->SetNewTarget(pos.x, pos.y);
					
					break;
				}
			}


		}

	}
}