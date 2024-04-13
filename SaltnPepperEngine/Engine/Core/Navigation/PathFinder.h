#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "NavMesh.h"
#include <queue>

namespace SaltnPepperEngine
{
	namespace Navigation
	{
        struct Node 
        {
            int index; // Index of the triangle 
            float f; // Total cost f
            bool operator<(const Node& other) const { return f > other.f; } // For priority queue
        };

        class Pathfinder
        {
        private:
            NavMesh* graph;
            vector<int> cameFrom;
            vector<float> gScore;

        public:
            Pathfinder(NavMesh* graph_) : graph(graph_) {}

            // Function to find the shortest path using A*
            vector<Vector3> findPath(const Vector3& start, const Vector3& goal) {
                int startNode = graph->getNearestTriangleIndex(start);
                int goalNode = graph->getNearestTriangleIndex(goal);

                // Initialize the open and closed sets
                priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> openSet;
                openSet.push(make_pair(0, startNode));

                cameFrom.assign(graph->getTriangleCount(), -1);
                gScore.assign(graph->getTriangleCount(), numeric_limits<float>::max());
                gScore[startNode] = 0;
            

                while (!openSet.empty()) {
                    int current = openSet.top().second;
                    openSet.pop();

                    if (current == goalNode) {
                        // Reconstruct the path
                        vector<Vector3> path;
                        while (current != startNode) {
                            path.push_back(graph->getTriangleCenter(current));
                            current = cameFrom[current];
                        }
                        path.push_back(start);
                        reverse(path.begin(), path.end());
                        return path;
                    }

                    for (int neighbor : graph->getNeighbors(current)) {
                        float tentative_gScore = gScore[current] + Distance(graph->getTriangleCenter(current), graph->getTriangleCenter(neighbor));

                        if (tentative_gScore < gScore[neighbor]) {
                            cameFrom[neighbor] = current;
                            gScore[neighbor] = tentative_gScore;
                            float fScore = tentative_gScore + Distance(graph->getTriangleCenter(neighbor), graph->getTriangleCenter(goalNode));
                            openSet.push(make_pair(fScore, neighbor));
                        }
                    }
                }

                // No path found
                return vector<Vector3>();
            }

        private:
          
           
           
        };
	}
}

#endif // !PATHFINDER_H
