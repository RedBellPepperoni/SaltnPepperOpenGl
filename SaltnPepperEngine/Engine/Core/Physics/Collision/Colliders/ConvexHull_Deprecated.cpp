#include "ConvexHull_Deprecated.h"
#include "Engine/Core/Rendering/Renderer/DebugRenderer.h"

namespace SaltnPepperEngine
{
	using namespace Rendering;

	namespace Physics
	{
		ConvexHull_Deprecated::ConvexHull_Deprecated()
		{
		}
		ConvexHull_Deprecated::~ConvexHull_Deprecated()
		{
		}


		void ConvexHull_Deprecated::AddVertex(const Vector3& v)
		{
			HullVertex new_vertex;
			new_vertex.idx = static_cast<int>(m_Vertices.size());
			new_vertex.pos = v;

			m_Vertices.push_back(new_vertex);
		}

		void ConvexHull_Deprecated::AddFace(const Vector3& normal, int nVerts, const int* verts)
		{
			HullFace new_face;
			new_face.idx = (int)m_Faces.size();
			new_face.normal = normal;
			new_face.normal = glm::normalize(new_face.normal);

			m_Faces.push_back(new_face);
			HullFace* new_face_ptr = &m_Faces[new_face.idx];

			// Construct all contained edges
			int p0 = nVerts - 1;
			for (int p1 = 0; p1 < nVerts; ++p1)
			{
				new_face_ptr->vert_ids.push_back(verts[p1]);
				new_face_ptr->edge_ids.push_back(ConstructNewEdge(new_face.idx, verts[p0], verts[p1]));
				p0 = p1;
			}

			// Find Adjacent Faces
			for (int i = 0; i < new_face.idx; ++i)
			{
				HullFace& cFace = m_Faces[i];
				bool found = false;
				for (size_t j = 0; found == false && j < cFace.edge_ids.size(); ++j)
				{
					for (int k = 0; found == false && k < nVerts; ++k)
					{
						if (new_face_ptr->edge_ids[k] == cFace.edge_ids[j])
						{
							found = true;
							cFace.adjoining_face_ids.push_back(new_face.idx);
							new_face_ptr->adjoining_face_ids.push_back(i);
						}
					}
				}
			}

			// Update Contained Vertices
			for (int i = 0; i < nVerts; ++i)
			{
				HullVertex* cVertStart = &m_Vertices[m_Edges[new_face_ptr->edge_ids[i]].vStart];
				HullVertex* cVertEnd = &m_Vertices[m_Edges[new_face_ptr->edge_ids[i]].vEnd];

				const auto foundLocStart = std::find(cVertStart->enclosing_faces.begin(), cVertStart->enclosing_faces.end(), new_face.idx);
				if (foundLocStart == cVertStart->enclosing_faces.end())
				{
					cVertStart->enclosing_faces.push_back(new_face.idx);
				}

				const auto foundLocEnd = std::find(cVertEnd->enclosing_faces.begin(), cVertEnd->enclosing_faces.end(), new_face.idx);
				if (foundLocEnd == cVertEnd->enclosing_faces.end())
				{
					cVertEnd->enclosing_faces.push_back(new_face.idx);
				}
			}
		}

		int ConvexHull_Deprecated::FindEdge(int v0_idx, int v1_idx)
		{
			for (const HullEdge& edge : m_Edges)
			{
				if ((edge.vStart == v0_idx && edge.vEnd == v1_idx)
					|| (edge.vStart == v1_idx && edge.vEnd == v0_idx))
				{
					return edge.idx;
				}
			}

			return -1;
		}

		void ConvexHull_Deprecated::GetMinMaxVerticesInAxis(const Vector3& local_axis, int* out_min_vert, int* out_max_vert)
		{

			int minVertex = 0, maxVertex = 0;

			float minCorrelation = FLT_MAX, maxCorrelation = -FLT_MAX;

			for (size_t i = 0; i < m_Vertices.size(); ++i)
			{
				const float cCorrelation = Dot(local_axis, m_Vertices[i].pos);

				if (cCorrelation > maxCorrelation)
				{
					maxCorrelation = cCorrelation;
					maxVertex = int(i);
				}

				if (cCorrelation <= minCorrelation)
				{
					minCorrelation = cCorrelation;
					minVertex = int(i);
				}
			}

			if (out_min_vert)
				*out_min_vert = minVertex;
			if (out_max_vert)
				*out_max_vert = maxVertex;
		}

		void ConvexHull_Deprecated::DebugDraw(const Matrix4& transform)
		{
			/*for (HullFace& face : m_Faces)
			{
				if (face.vert_ids.size() > 2)
				{
					Vector3 polyginStart = transform * Vector4(m_Vertices[face.vert_ids[0]].pos, 1.0f);
					Vector3 polyginEnd = transform * Vector4(m_Vertices[face.vert_ids[1]].pos, 1.0f);
				
					for (size_t index = 2; index < face.vert_ids.size(); ++index)
					{
						Vector3 nextPolygon = transform * Vector4(m_Vertices[face.vert_ids[index]].pos, 1.0f);
					}
				
				
				
				}
			}*/

			for (HullEdge& edge : m_Edges)
			{
				DebugRenderer::DrawLine(transform * Vector4(m_Vertices[edge.vStart].pos, 1.0f), transform * Vector4(m_Vertices[edge.vEnd].pos, 1.0f),  Vector4(0.7f, 0.2f, 0.7f, 1.0f) , 0.02f);
			}
		}

		

		int ConvexHull_Deprecated::ConstructNewEdge(int parent_face_idx, int vert_start, int vert_end)
		{
			int out_idx = FindEdge(vert_start, vert_end);

			// Edge not already within the Hull,
			if (out_idx == -1)
			{
				out_idx = static_cast<int>(m_Edges.size());

				HullEdge new_edge;
				new_edge.idx = static_cast<int>(m_Edges.size());
				new_edge.vStart = vert_start;
				new_edge.vEnd = vert_end;
				m_Edges.push_back(new_edge);

				HullEdge* new_edge_ptr = &m_Edges[new_edge.idx];

				// Find Adjacent Edges
				for (int i = 0; i < new_edge.idx; ++i)
				{
					if (m_Edges[i].vStart == vert_start
						|| m_Edges[i].vStart == vert_end
						|| m_Edges[i].vEnd == vert_start
						|| m_Edges[i].vEnd == vert_end)
					{
						m_Edges[i].adjoining_edge_ids.push_back(new_edge.idx);
						new_edge_ptr->adjoining_edge_ids.push_back(i);
					}
				}

				// Update Contained Vertices
				m_Vertices[vert_start].enclosing_edges.push_back(new_edge.idx);
				m_Vertices[vert_end].enclosing_edges.push_back(new_edge.idx);
			}

			m_Edges[out_idx].enclosing_faces.push_back(parent_face_idx);
			return out_idx;
		}
	}
}

