

#include "NavMesh.h"


namespace SaltnPepperEngine
{
	namespace Navigation
	{
		NavMesh::NavMesh(const vector<NavTriangle>& trianglesList)
            : triangles(trianglesList)
        {
            // Generate adjacency list
            for (int i = 0; i < triangles.size(); ++i) {
                for (int j = 0; j < triangles.size(); ++j) {
                    if (i != j && trianglesShareEdge(triangles[i], triangles[j])) {
                        triangles[i].neighbors.push_back(j);
                    }
                }
            }
        }

        bool NavMesh::trianglesShareEdge(const NavTriangle& t1, const NavTriangle& t2)
        {
            int sharedVertices = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (t1.vertices[i].x == t2.vertices[j].x && t1.vertices[i].y == t2.vertices[j].y &&
                        t1.vertices[i].z == t2.vertices[j].z) {
                        sharedVertices++;
                    }
                }
            }
            return sharedVertices == 2;
        }

        int NavMesh::getNearestTriangleIndex(const Vector3& point) const
        {
            int nearestIndex = -1;
            float minDistance = numeric_limits<float>::max();

            for (int i = 0; i < triangles.size(); ++i) {
                float distance = sqrt(pow(point.x - triangles[i].center.x, 2) +
                    pow(point.y - triangles[i].center.y, 2) +
                    pow(point.z - triangles[i].center.z, 2));

                if (distance < minDistance) {
                    minDistance = distance;
                    nearestIndex = i;
                }
            }

            return nearestIndex;
        }




	}
}