#ifndef NAVMESH_H
#define NAVMESH_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>
#include <unordered_map>

using namespace std;

namespace SaltnPepperEngine
{
	namespace Navigation
	{

        struct NavTriangle
        {
			Vector3 center;
			Vector3 vertices[3];

			vector<int> neighbors;

			NavTriangle(const Vector3& vertexOne, const Vector3& vertexTwo, const Vector3& vertexThree)
			{
				vertices[0] = vertexOne;
				vertices[1] = vertexTwo;
				vertices[2] = vertexThree;

				center = CalculateCenter();
			}

			// Function to calculate the center of the triangle
			Vector3 CalculateCenter() const
			{
				Vector3 center(0, 0, 0);
				for (int i = 0; i < 3; ++i) {
					center.x += vertices[i].x;
					center.y += vertices[i].y;
					center.z += vertices[i].z;
				}
				center.x /= 3;
				center.y /= 3;
				center.z /= 3;
				return center;
			}

        };

		class NavMesh
		{

		private:
			vector<NavTriangle> triangles;

        public:
            NavMesh(const vector<NavTriangle>& trianglesList);
             

            // Function to check if two triangles share an edge
			bool trianglesShareEdge(const NavTriangle& t1, const NavTriangle& t2);

            // Function to get the index of the nearest triangle to a given point
			int getNearestTriangleIndex(const Vector3& point) const;

            // Function to get the neighbors of a triangle

            const vector<int>& getNeighbors(int triangleIndex) const
			{
                return triangles[triangleIndex].neighbors;
            }

            // Function to get the center of a triangle
            Vector3 getTriangleCenter(int triangleIndex) const
			{
                return triangles[triangleIndex].center;
            }

			int getTriangleCount() const {
				return static_cast<int>(triangles.size());
			}


		};
	}
}


#endif // !NAVMESH_H
