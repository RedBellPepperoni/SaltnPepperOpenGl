#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "NavMesh.h"
#include <queue>

namespace SaltnPepperEngine
{
	namespace Navigation
	{
       
       

        class Pathfinder
        {
        private:
            NavMesh* m_graph;
            vector<int> m_cameFrom;
            vector<float> m_gScore;

        public:
            Pathfinder(NavMesh* graph);

            vector<Vector3> FindSimplfiedPath(const Vector3& start, const Vector3& goal);
        
            // shortest path using A*
            vector<Vector3> FindPath(const Vector3& start, const Vector3& goal);
             //  Path Simplification using Funnel Algorithm
            vector<Vector3> SimplifyPath(const vector<Vector3>& path, const NavMesh* graph) const;
    
            vector<Vector3> SimplifyDouglus(const vector<Vector3>& path, const float epsilon) const;
           
        };
	}
}

#endif // !PATHFINDER_H
