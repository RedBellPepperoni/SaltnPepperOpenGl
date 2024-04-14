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
			return SimplifyPath(path, m_graph);
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

        vector<Vector3> Pathfinder::SimplifyPath(const vector<Vector3>& path, const NavMesh* graph) const
        {
            if (path.empty()) { return path; }

            vector<Vector3> centers = path;


            if (centers.size() < 2) {
                return centers;
            }


            Funnel funnel(centers[0], centers[1]);
            vector<Vector3> simplifiedCenters = { centers[0] };


            for (size_t i = 2; i < centers.size(); ++i) {

                if (funnel.isInside(centers[i])) {

                    funnel.UpdateRight(centers[i]);
                }
                else {

                    simplifiedCenters.push_back(funnel.GetLeft());
                    funnel = Funnel(funnel.GetRight(), centers[i]);
                }
            }


            simplifiedCenters.push_back(centers.back());

            return simplifiedCenters;
        }
		
	}
}