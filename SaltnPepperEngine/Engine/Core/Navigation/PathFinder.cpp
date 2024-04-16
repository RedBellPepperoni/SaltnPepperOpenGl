#include "PathFinder.h"

using namespace std;

namespace SaltnPepperEngine
{
	namespace Navigation
	{
		Pathfinder::Pathfinder(NavMesh* graph)
			: m_graph(graph) 
		{}

		vector<Vector3> Pathfinder::FindSimplfiedPath(const Vector3 & start, const Vector3 & goal)
		{
			// Get the Path
			vector<Vector3> path = FindPath(start, goal);

			// Return the Simplified path usinf funnel
			//return SimplifyPath(path, m_graph);
            const float epsilon = 3.0f;

			return SimplifyDouglus(path, epsilon);
		}

		vector<Vector3> Pathfinder::FindPath(const Vector3& start, const Vector3& goal)
		{
            m_gScore.clear();
            m_cameFrom.clear();

            int startNode = GetGraph()->GetNearestTriangleIndex(start);
            int goalNode = GetGraph()->GetNearestTriangleIndex(goal);


            priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> openSet;
            openSet.push(make_pair(0.0f, startNode));

            m_cameFrom.assign(GetGraph()->GetTriangleCount(), -1);
            m_gScore.assign(GetGraph()->GetTriangleCount(), numeric_limits<float>::max());
            m_gScore[startNode] = 0;


            while (!openSet.empty())
            {
                int current = openSet.top().second;
                openSet.pop();

                if (current == goalNode)
                {
                    // Reconstruct the path
                    vector<Vector3> path;

                    while (current != startNode) 
                    {
                        path.push_back(GetGraph()->GetTriangleCenter(current));
                        current = m_cameFrom[current];
                    }

                    path.push_back(start);
                    reverse(path.begin(), path.end());
                    return path;
                }

                for (int neighbor : GetGraph()->GetNeighbors(current))
                {
                    float tentative_gScore = m_gScore[current] + Distance(GetGraph()->GetTriangleCenter(current), GetGraph()->GetTriangleCenter(neighbor));

                    if (tentative_gScore < m_gScore[neighbor]) 
                    {
                        m_cameFrom[neighbor] = current;
                        m_gScore[neighbor] = tentative_gScore;
                        float fScore = tentative_gScore + Distance(GetGraph()->GetTriangleCenter(neighbor), GetGraph()->GetTriangleCenter(goalNode));
                        openSet.push(make_pair(fScore, neighbor));
                    }
                }
            }

            // No path found
            return vector<Vector3>{};
		}


        

        vector<Vector3> Pathfinder::SimplifyPath(const vector<Vector3>& path, const NavMesh* graph) const
        {
            return vector<Vector3>{};
        }

        // Douglas-Peucker simplification algorithm
        void DouglasPeuckerSimplify(const std::vector<Vector3>& path, float epsilon, int startIndex, int endIndex, std::vector<Vector3>& simplifiedPath) {
            float dmax = 0;
            int index = -1;

          
            for (int i = startIndex + 1; i < endIndex; ++i) {
                float d = DistanceSquared(path[i], path[startIndex]) + DistanceSquared(path[i], path[endIndex]);
                if (d > dmax) {
                    index = i;
                    dmax = d;
                }
            }

            
            if (dmax > epsilon * epsilon)
            {
                DouglasPeuckerSimplify(path, epsilon, startIndex, index, simplifiedPath);
                DouglasPeuckerSimplify(path, epsilon, index, endIndex, simplifiedPath);
            }
            else 
            {
                
                simplifiedPath.push_back(path[endIndex]);
            }
        }

        vector<Vector3> Pathfinder::SimplifyDouglus(const vector<Vector3>& path, const float epsilon) const
        {
            std::vector<Vector3> simplifiedPath;


            // Perform the Douglas-Peucker simplification
            DouglasPeuckerSimplify(path, epsilon, 0, static_cast<int>(path.size() - 1), simplifiedPath);

            return simplifiedPath;
        }
		
	}
}