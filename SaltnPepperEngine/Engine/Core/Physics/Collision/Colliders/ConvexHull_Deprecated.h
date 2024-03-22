#pragma once
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{

	namespace Rendering
	{
		class Mesh;
	}

	using Rendering::Mesh;

	namespace Physics
	{

		// ============ Work on this later to simplfy mesh triangulation convex hull
        struct HullVertex
        {
            int idx = 0;
            Vector3 pos;
            std::vector<int> enclosing_edges;
            std::vector<int> enclosing_faces;
        };

        struct HullEdge
        {
            int idx = 0;
            int vStart = 0, vEnd = 0;
            std::vector<int> adjoining_edge_ids;
            std::vector<int> enclosing_faces;
        };

        struct HullFace
        {
            int idx = 0;
            Vector3 normal;
            std::vector<int> vert_ids;
            std::vector<int> edge_ids;
            std::vector<int> adjoining_face_ids;
        };

        class ConvexHull_Deprecated
        {
        public:
            ConvexHull_Deprecated();
            ~ConvexHull_Deprecated();


            void AddVertex(const Vector3& v);

            void AddFace(const Vector3& normal, int nVerts, const int* verts);
            void AddFace(const Vector3& normal, const std::vector<int>& vert_ids) { AddFace(normal, static_cast<int>(vert_ids.size()), &vert_ids[0]); }

            int FindEdge(int v0_idx, int v1_idx);

            const HullVertex& GetVertex(int idx) { return m_Vertices[idx]; }
            const HullEdge& GetEdge(int idx) { return m_Edges[idx]; }
            const HullFace& GetFace(int idx) { return m_Faces[idx]; }

            size_t GetNumVertices() const { return m_Vertices.size(); }
            size_t GetNumEdges() const { return m_Edges.size(); }
            size_t GetNumFaces() const { return m_Faces.size(); }

            void GetMinMaxVerticesInAxis(const Vector3& local_axis, int* out_min_vert, int* out_max_vert);

            void DebugDraw(const Matrix4& transform);

        protected:
            int ConstructNewEdge(int parent_face_idx, int vert_start, int vert_end); // Called by AddFace

        protected:
            std::vector<HullVertex> m_Vertices;
            std::vector<HullEdge> m_Edges;
            std::vector<HullFace> m_Faces;
        };

	}
}

