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
            const float epsilon = 4.0f;

			return SimplifyDouglus(path, epsilon);
		}

		vector<Vector3> Pathfinder::FindPath(const Vector3& start, const Vector3& goal)
		{
            int startNode = m_graph->GetNearestTriangleIndex(start);
            int goalNode = m_graph->GetNearestTriangleIndex(goal);


            priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> openSet;
            openSet.push(make_pair(0.0f, startNode));

            m_cameFrom.assign(m_graph->GetTriangleCount(), -1);
            m_gScore.assign(m_graph->GetTriangleCount(), numeric_limits<float>::max());
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
                        path.push_back(m_graph->GetTriangleCenter(current));
                        current = m_cameFrom[current];
                    }

                    path.push_back(start);
                    reverse(path.begin(), path.end());
                    return path;
                }

                for (int neighbor : m_graph->GetNeighbors(current))
                {
                    float tentative_gScore = m_gScore[current] + Distance(m_graph->GetTriangleCenter(current), m_graph->GetTriangleCenter(neighbor));

                    if (tentative_gScore < m_gScore[neighbor]) 
                    {
                        m_cameFrom[neighbor] = current;
                        m_gScore[neighbor] = tentative_gScore;
                        float fScore = tentative_gScore + Distance(m_graph->GetTriangleCenter(neighbor), m_graph->GetTriangleCenter(goalNode));
                        openSet.push(make_pair(fScore, neighbor));
                    }
                }
            }

            // No path found
            return vector<Vector3>();
		}


        // Calculate the signed area of a triangle
        float SignedArea(const Vector3& a, const Vector3& b, const Vector3& c) 
        {
            return Dot(Cross((b - a), (c- b)), Vector3{ 0, 1, 0 });
        }

        vector<Vector3> Pathfinder::SimplifyPath(const vector<Vector3>& path, const NavMesh* graph) const
        {
           
        }

        // Douglas-Peucker simplification algorithm
        void DouglasPeuckerSimplify(const std::vector<Vector3>& path, float epsilon, int startIndex, int endIndex, std::vector<Vector3>& simplifiedPath) {
            float dmax = 0;
            int index = -1;

            // Find the point with the maximum distance
            for (int i = startIndex + 1; i < endIndex; ++i) {
                float d = DistanceSquared(path[i], path[startIndex]) + DistanceSquared(path[i], path[endIndex]);
                if (d > dmax) {
                    index = i;
                    dmax = d;
                }
            }

            // If the maximum distance is greater than epsilon, recursively simplify
            if (dmax > epsilon * epsilon)
            {
                DouglasPeuckerSimplify(path, epsilon, startIndex, index, simplifiedPath);
                DouglasPeuckerSimplify(path, epsilon, index, endIndex, simplifiedPath);
            }
            else 
            {
                // Add the simplified point
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