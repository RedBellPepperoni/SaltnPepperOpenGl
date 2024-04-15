#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "NavMesh.h"
#include <queue>

namespace SaltnPepperEngine
{
	namespace Navigation
	{
       
        class Funnel 
        {
        private:
            Vector3 left;
            Vector3 right;

        public:
            Funnel(const Vector3& p1, const Vector3& p2) : left(p1), right(p2) {}

            inline bool isInside(const Vector3& point) const { return Cross(right - left, point - left).z >= 0;}

            inline void UpdateRight(const Vector3& newPoint) {right = newPoint;}

            inline const Vector3& GetLeft() const { return left;}
            inline const Vector3& GetRight() const { return right; }
        };


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
    
           
        };
	}
}

#endif // !PATHFINDER_H
