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

        class Funnel {
        private:
            Vector3 left;
            Vector3 right;

        public:
            Funnel(const Vector3& p1, const Vector3& p2) : left(p1), right(p2) {}


            bool isInside(const Vector3& point) const {
                return Cross(right - left, point - left).z >= 0;
            }

            void updateRight(const Vector3& newPoint) {
                right = newPoint;
            }

         
            const Vector3& getLeft() const {
                return left;
            }

           
            const Vector3& getRight() const {
                return right;
            }
        };


        class Pathfinder
        {
        private:
            NavMesh* graph;
            vector<int> cameFrom;
            vector<float> gScore;

        public:
            Pathfinder(NavMesh* graph_) : graph(graph_) {}

            vector<Vector3> FindSimplfiedPath(const Vector3& start, const Vector3& goal)
            {
                vector<Vector3> path = findPath(start, goal);

                return simplifyPath(path,graph);
            }

            // Function to find the shortest path using A*
            vector<Vector3> findPath(const Vector3& start, const Vector3& goal) {
                int startNode = graph->getNearestTriangleIndex(start);
                int goalNode = graph->getNearestTriangleIndex(goal);

             
                priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> openSet;
                openSet.push(make_pair(0.0f, startNode));

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


            vector<Vector3> simplifyPath(const vector<Vector3>& path, const NavMesh* graph) const {
                vector<Vector3> centers = path;
                if (centers.size() < 2) {
                    return centers; 
                }

             
                Funnel funnel(centers[0], centers[1]);
                vector<Vector3> simplifiedCenters = { centers[0] };

             
                for (size_t i = 2; i < centers.size(); ++i) {
                   
                    if (funnel.isInside(centers[i])) {
                      
                        funnel.updateRight(centers[i]);
                    }
                    else {
                       
                        simplifiedCenters.push_back(funnel.getLeft());
                        funnel = Funnel(funnel.getRight(), centers[i]);
                    }
                }

              
                simplifiedCenters.push_back(centers.back());

                return simplifiedCenters;
            }

        private:
          
           
           
        };
	}
}

#endif // !PATHFINDER_H
