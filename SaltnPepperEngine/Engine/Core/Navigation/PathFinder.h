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
        public:
            // Constructor
            Pathfinder(const NavMesh& navMesh) : m_navMesh(navMesh) 
            {}

            std::vector<Vector3> FndPath(const Vector3& startPos, const Vector3& goalPos) const {
                // Find the nearest triangles to the start and goal positions
                int startIndex = m_navMesh.FindContainingTriangle(startPos);
                int goalIndex = m_navMesh.FindContainingTriangle(goalPos);

                if (startIndex == -1 || goalIndex == -1) {
                    // Either start or goal position is not inside any triangle
                    return {};
                }

                const size_t NavmeshSize = m_navMesh.GetNavMesh().size();

                std::priority_queue<Node> openSet;
                std::vector<bool> closedSet(NavmeshSize, false);
                std::vector<float> gScore(NavmeshSize, std::numeric_limits<float>::infinity());
                std::vector<int> cameFrom(NavmeshSize, -1);

                gScore[startIndex] = 0;
                openSet.push({ startIndex, distance(startPos, goalPos) });

                while (!openSet.empty()) {
                    Node current = openSet.top();
                    openSet.pop();

                    if (current.index == goalIndex) {
                        // Reconstruct path
                        std::vector<Vector3> path;
                        int currentIndex = goalIndex;
                        while (currentIndex != startIndex) {
                            path.push_back(m_navMesh.CalculateTriangleCenter(currentIndex));
                            currentIndex = cameFrom[currentIndex];
                        }
                        path.push_back(m_navMesh.CalculateTriangleCenter(startIndex));
                        std::reverse(path.begin(), path.end());
                        return path;
                    }

                    closedSet[current.index] = true;

                    for (int neighborIndex = 0; neighborIndex < NavmeshSize; ++neighborIndex) {
                        if (!closedSet[neighborIndex]) {
                            float tentativeGScore = gScore[current.index] + Distance(m_navMesh.CalculateTriangleCenter(current.index), m_navMesh.CalculateTriangleCenter(neighborIndex));
                            if (tentativeGScore < gScore[neighborIndex]) {
                                cameFrom[neighborIndex] = current.index;
                                gScore[neighborIndex] = tentativeGScore;
                                float fScore = gScore[neighborIndex] + Distance(m_navMesh.CalculateTriangleCenter(neighborIndex), goalPos);
                                openSet.push({ neighborIndex, fScore });
                            }
                        }
                    }
                }

                return {}; // No path found
            }

        private:
            // Member variables
             NavMesh m_navMesh;
        };
	}
}

#endif // !PATHFINDER_H
