#ifndef NAVMESH_H
#define NAVMESH_H

#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{
	namespace Navigation
	{

        struct NavTriangle
        {
            int indices[3];
        };

		class NavMesh
		{
        public:
          
            NavMesh(const std::vector<Vector3>& vertices, const std::vector<NavTriangle>& triangles) 
                : m_vertices(vertices)
                , m_navMesh(triangles)
            {}

          
            int FindContainingTriangle(const Vector3& position) const 
            {
                for (size_t i = 0; i < m_navMesh.size(); ++i) {
                    const Vector3& v0 = m_vertices[m_navMesh[i].indices[0]];
                    const Vector3& v1 = m_vertices[m_navMesh[i].indices[1]];
                    const Vector3& v2 = m_vertices[m_navMesh[i].indices[2]];
                    
                    // Check if the world position is inside the triangle
                    if (PointInTriangle3D(position, v0, v1, v2)) {
                        return i;
                    }
                }

                // Position not inside any triangle
                return -1; 
            }

            

           
            Vector3 CalculateTriangleCenter(int triangleIndex) const {
                const NavTriangle& triangle = m_navMesh[triangleIndex];
                const Vector3& v0 = m_vertices[triangle.indices[0]];
                const Vector3& v1 = m_vertices[triangle.indices[1]];
                const Vector3& v2 = m_vertices[triangle.indices[2]];

              
                return Vector3((v0.x + v1.x + v2.x) / 3.0f, (v0.y + v1.y + v2.y) / 3.0f, (v0.z + v1.z + v2.z) / 3.0f);
            }

            const std::vector<NavTriangle>& GetNavMesh() const { return m_navMesh; }
            const std::vector<Vector3>& GetVertices() const { return m_vertices; }

        private:
            
            bool PointInTriangle3D(const Vector3& p, const Vector3& v0, const Vector3& v1, const Vector3& v2) const 
            {
                Vector3 v0v1 = v1 - v0;
                Vector3 v0v2 = v2 - v0;
                Vector3 vp = p - v0;

               
                float dot00 = Dot(v0v1 , v0v1);
                float dot01 = Dot(v0v1 , v0v2);
                float dot02 = Dot(v0v1 , vp);
                float dot11 = Dot(v0v2 , v0v2);
                float dot12 = Dot(v0v2 , vp);

                //  barycentric coordinates
                float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
                float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
                float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

                return (u >= 0) && (v >= 0) && (u + v <= 1);
            }

            // Member variables
            std::vector<Vector3> m_vertices;
            std::vector<NavTriangle> m_navMesh;
		};
	}
}


#endif // !NAVMESH_H
