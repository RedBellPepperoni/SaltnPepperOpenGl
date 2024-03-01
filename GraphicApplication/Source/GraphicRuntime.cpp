#include "SaltnPepperEngine.h"
#include "EntitySetup.h"
#include "Engine/Core/Threading/MultiThreader.h"
#include "DungeonManager.h"
#include "PathFinder.h"
#include "AstarFinder.h"
#include "Graph.h"
#include "GraphNode.h"


class GraphicRuntime : public Application
{
   

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
      
     
        hunterT = CreateHunter(Vector3(1.0f,0.0f,1.0f));

        
        Entity mainCamera = CreateMainCamera(Vector3(-17.3f,218.02f,56.26f),Vector3(-13.17f,24.86,0.00f));

        Camera* cam = &mainCamera.GetComponent<Camera>();

        auto euclideanDistance = [](GraphNode* pNode, Graph* pGraph)
            {
                auto pEnd = pGraph->GetEndNode();
                return Length(pEnd->GetPosition() - pNode->GetPosition());
            };

        dungeonGraph = MakeShared<Graph>();


        dungeonGraph->Create(manager->GetWallData(), 115, 149, 1.0f, 1.0f);
        dungeonGraph->SetHeuristicFunction(euclideanDistance);

        hunter = MakeUnique<AstarFinder>();

        SharedPtr<Graph> copy = MakeShared<Graph>(*dungeonGraph);

        hunter->SetBegin(GetIndex(1, 1));
        hunter->SetEnd(GetIndex(13, 12));
        hunter->FindPath(copy);

        std::deque<GraphNode*> path = hunter->GetPath();

        for (auto& node : path)
        {
           //LOG_WARN("NODE : {0}, {1}", node->GetPosition().x, node->GetPosition().y);

            Vector2 nodePos = Vector2(node->GetPosition().x, node->GetPosition().y);

            nodes.push_back(nodePos);
        }

        //pathCounter = path.begin();
	}

    int GetIndex(int x, int y)
    {
       return x + y * 115;
    }

	void OnUpdate(float deltaTime)
	{
        

        counter += deltaTime;

        if (counter > timer)
        {
            counter = 0.0f;

           // std::deque<GraphNode*> path = hunter->GetPath();



            if (nodes.empty())
            {
                LOG_ERROR("No Path Found");
                return;
            }

           // if (pathCounter < path.end())
            if (pathCounter < nodes.size())
            {
                Vector2 posNode = nodes[pathCounter];

                Vector3 pos = Vector3(posNode.x,0.0f,posNode.y);
                //  Vector3 pos = hunterT->GetPosition();

               // pos = pos + Vector3(1.0f, 0.0f, 0.0f);

                LOG_WARN("NODE : {0}, {1}", posNode.x, posNode.y);

                hunterT->SetPosition(pos);

                pathCounter++;
            }
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