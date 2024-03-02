#ifndef HUNTERMANAGER_H
#define HUNTERMANAGER_H
#include "SaltnPepperEngine.h"
#include "DungeonManager.h"
#include "PathFinder.h"
#include "AstarFinder.h"
#include "Graph.h"
#include "GraphNode.h"
#include "TreasureHunter.h"
#include "HunterThreaded.h"

namespace SaltnPepperEngine
{

	class HunterManager
	{
    public:

        void CreateDungeon();
        void CreateHunters(int count);
        void LaunchHunters();
        void CreateTreasures(int count);

        Vector3 GetPositionfromIndex(int index);   

        int GetRandomFloorIndex();
     

        Transform* SpawnRandomTreasure();
    
        Vector2Int GetRandomAvailableTreasure();

        void MovetoNextTreaure(SharedPtr<TreasureHunter>& hun);
    
        void UsedUpTreasure(const Vector3 position);
        void UsedUpTreasure(const int index);
     
        const int GetIndex(int x, int y) const
        {
            return x + (y * 115);
        }

        void Update(const float deltaTime);

    private:

        int pathCounter = 0;
        // std::deque<GraphNode*>::iterator pathCounter;
        std::vector<Vector2> nodes;
        SharedPtr<DungeonManager> manager;
        UniquePtr<PathFinder> hunter;
       
        SharedPtr<Graph> dungeonGraph;

        std::vector<SharedPtr<TreasureHunter>> hunters;
        std::vector<SharedPtr<HunterThreadInfo>> threadInfos;

        std::vector<HANDLE> threadhandleList;

        std::vector<Transform*> availableTreasures;
        std::vector<Transform*> pickedTreasures;

        float timer = 1.0f;
        float counter = 0.0f;

	};

}
#endif // !HUNTERMANAGER_H

