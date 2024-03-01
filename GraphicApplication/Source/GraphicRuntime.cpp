#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "DungeonManager.h"
#include "PathFinder.h"
#include "AstarFinder.h"
#include "Graph.h"
#include "GraphNode.h"
#include "TreasureHunter.h"


class GraphicRuntime : public Application
{

    Vector3 GetPositionfromIndex(int index)
    {
        int y = index / 115;
        int x = index % 115;

        return Vector3(x, 0.0f, y);
    }

    int GetRandomFloorIndex()
    {
        int index = Random32::Range.GetRandom(0, dungeonGraph->GetNodes().size()-1);

        if (dungeonGraph->GetNodes()[index]->IsBlocked() == false)
        {
            return index;
        }

        
        return GetRandomFloorIndex();
    }
   
    Transform* SpawnRandomTreasure()
    {
        int randomIndex = GetRandomFloorIndex();

        Vector3 spawnPos = GetPositionfromIndex(randomIndex);

        Transform* tr = CreateTreasure(spawnPos);

        return tr;
    }



    void OnInit()
    {
       
       
        LoadAllModels();
        LoadAllTextures();

       
        MultiThreader::Init();

     
        glPolygonMode(GL_FRONT, GL_FILL);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
      

        manager = MakeShared<DungeonManager>();

        manager->LoadTSVMap("Assets\\Data\\DungeonCleanCompact.tsv");
        manager->SetStartandEnd();

        //manager->PrintMap();
        manager->Create3DMap();
        CreateDirectionalLight();
      
     
       

        
        Entity mainCamera = CreateMainCamera(Vector3(19.3f,23.01f,14.20f),Vector3(-75.85,-0.67,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();

        auto euclideanDistance = [](GraphNode* pNode, Graph* pGraph)
            {
                auto pEnd = pGraph->GetEndNode();
                return Length(pEnd->GetPosition() - pNode->GetPosition());
            };

        dungeonGraph = MakeShared<Graph>();


        dungeonGraph->Create(manager->GetWallData(), 115, 149, 1.0f, 1.0f);
        dungeonGraph->SetHeuristicFunction(euclideanDistance);

        //hunter = MakeUnique<AstarFinder>();

        //SharedPtr<Graph> copy = MakeShared<Graph>(*dungeonGraph);

        //hunter->SetBegin(GetIndex(1, 1));
        //hunter->SetEnd(GetIndex(13, 12));
        //hunter->FindPath(copy);

        //std::deque<GraphNode*> path = hunter->GetPath();

        //for (auto& node : path)
        //{
        //   //LOG_WARN("NODE : {0}, {1}", node->GetPosition().x, node->GetPosition().y);

        //    Vector2 nodePos = Vector2(node->GetPosition().x, node->GetPosition().y);

        //    nodes.push_back(nodePos);
        //}

        ////pathCounter = path.begin();

        for (int i = 0; i < 1; i++)
        {
           SharedPtr<TreasureHunter> hunter = MakeShared<TreasureHunter>();
           Transform*  T = CreateHunter(Vector3(1.0f, 0.0f, 1.0f));
           hunter->SetGraph(dungeonGraph.get());
           hunter->SetTransform(T);
           hunter->SetSpawn(1,1);

           hunters.push_back(hunter);

        }

        hunters[0]->SetNewTarget(12, 13);
      /*  hunters[1]->SetNewTarget(20, 3);
        hunters[2]->SetNewTarget(21, 1);
        hunters[3]->SetNewTarget(19, 14);
        hunters[4]->SetNewTarget(12, 12);*/


        for (int i = 0; i < 250; i++)
        {
            Transform* treasturetransform = SpawnRandomTreasure();

            availableTreasures.push_back(treasturetransform);
        }
       

	}

    int GetIndex(int x, int y)
    {
       return x + y * 115;
    }

	void OnUpdate(float deltaTime)
	{
        for (SharedPtr<TreasureHunter>& hun : hunters)
        {
            hun->Update(deltaTime);
        }

        
	}


private:
    int pathCounter = 0;
   // std::deque<GraphNode*>::iterator pathCounter;
    std::vector<Vector2> nodes;
    SharedPtr<DungeonManager> manager;
    UniquePtr<PathFinder> hunter;
    Transform* hunterT = nullptr;

    SharedPtr<Graph> dungeonGraph;

    std::vector<SharedPtr<TreasureHunter>> hunters;


    std::vector<Transform*> availableTreasures;
    std::vector<Transform*> pickedTreasures;

    float timer = 1.0f;
    float counter = 0.0f;
};





int main(int argc, char* argv)
{
    // Creating a new Appinstance
    GraphicRuntime* app = new GraphicRuntime();

    //FilePath path = File::GetCurrentPath();

    // Always Initialize the App
    app->Initialize();
    // Running the Application
    app->Run();


    delete app;
}