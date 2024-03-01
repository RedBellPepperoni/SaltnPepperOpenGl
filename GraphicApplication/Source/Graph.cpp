#include "Graph.h"
#include "Engine/Utils/Logging/Log.h"

namespace SaltnPepperEngine
{
#include <algorithm>

	Graph::Graph() :
		m_iRows{ 0 },
		m_iCols{ 0 },
		m_pBegin{ nullptr },
		m_pEnd{ nullptr },
		m_pConnections{ nullptr }
	{
	}

	Graph::Graph(const Graph& graph)
	{
		*this = graph;
	}

	Graph& Graph::operator=(const Graph& graph)
	{
		m_iRows = graph.m_iRows;
		m_iCols = graph.m_iRows;

		m_nodes.reserve(graph.m_nodes.size());

		for (auto& pNode : graph.m_nodes)
		{
			auto pNewNode = std::make_unique<GraphNode>(*pNode);
			pNewNode->SetGraph(this);
			m_nodes.push_back(std::move(pNewNode));
		}

		m_heuristicFunc = graph.m_heuristicFunc;

		if (graph.m_pBegin)
			SetBegin(graph.m_pBegin->GetID());
		else
			m_pBegin = nullptr;

		if (graph.m_pEnd)
			SetBegin(graph.m_pEnd->GetID());
		else
			m_pEnd = nullptr;

		m_pConnections = &const_cast<Graph&>(graph).m_connections;

		return *this;
	}

	Graph::~Graph()
	{
	}

	GraphNode* Graph::GetNode(id_type id)
	{
		return ((int)id >= 0 && (int)id < (m_iRows * m_iCols)) ? m_nodes[id].get() : nullptr;
	}

	GraphNode* Graph::GetBeginNode()
	{
		return m_pBegin;
	}

	GraphNode* Graph::GetEndNode()
	{
		return m_pEnd;
	}

	Graph::ConnectionsMap Graph::GetConnections()
	{
		return m_pConnections ? *m_pConnections : m_connections;
	}

	std::vector<GraphNode*> Graph::GetNodeConnections(GraphNode* pNode)
	{
		auto nodeConnections = m_pConnections ? m_pConnections->find(pNode->GetID()) : m_connections.find(pNode->GetID());

		std::vector<GraphNode*> connections;

		auto connEnd = m_pConnections ? m_pConnections->end() : m_connections.end();

		if (nodeConnections != connEnd)
		{
			for (auto& connection_id : nodeConnections->second)
			{
				connections.emplace_back(GetNode(connection_id));
			}
		}

		//return std::move(connections);
		return (connections);
	}

	std::vector<GraphNode*> Graph::GetNodes()
	{
		std::vector<GraphNode*> out;

		out.reserve(m_nodes.size());

		for (auto& pNode : m_nodes)
			out.emplace_back(pNode.get());

		return std::move(out);
	}

	void Graph::Create(int iCols, int iRows, float fNodeWidth, float fNodeHeight)
	{
		Clean();

		m_iRows = iRows;
		m_iCols = iCols;

		m_nodes.reserve(iCols * iRows);

		for (int i = 0; i < (iCols * iRows); ++i)
		{
			auto pNode = std::make_unique<GraphNode>(i, this);
			pNode->SetPosition({ i % iCols * fNodeWidth,
									i / iCols * fNodeHeight, 0.f, 1.f });
			m_nodes.push_back(std::move(pNode));
		}

		for (int i = 0; i < (iCols * iRows); ++i)
		{
			const int x = i % iCols;
			const int y = i / iCols;

			const int iLeft = std::max(x - 1, 0);
			const int iRight = std::min(x + 1, iCols - 1);
			const int iTop = std::max(y - 1, 0);
			const int iDown = std::min(y + 1, iRows - 1);

			AddConnection(iLeft + y * iCols, i, true);
			AddConnection(iRight + y * iCols, i, true);
			AddConnection(iTop * iCols + x, i, true);
			AddConnection(iDown * iCols + x, i, true);

		}
	}

	void Graph::Create(std::vector<bool>& mapdata, int iCols, int iRows, float fNodeWidth, float fNodeHeight)
	{
		Clean();

		m_iRows = iRows;
		m_iCols = iCols;

		size_t dataSize = (size_t)iRows * (size_t)iCols;

		if (dataSize > mapdata.size())
		{
			LOG_CRITICAL("ROW + Colums Exceed Vector size");
		}


		m_nodes.reserve(dataSize);

		for (int i = 0; i < (dataSize); ++i)
		{
			auto pNode = std::make_unique<GraphNode>(i, this);
			pNode->SetPosition({ i % iCols * fNodeWidth,
									i / iCols * fNodeHeight, 0.f, 1.f });

			pNode->SetBlocked(mapdata[i]);

			m_nodes.push_back(std::move(pNode));


		}

		for (int i = 0; i < (iCols * iRows); ++i)
		{
			const int x = i % iCols;
			const int y = i / iCols;

			const int iLeft = std::max(x - 1, 0);
			const int iRight = std::min(x + 1, iCols - 1);
			const int iTop = std::max(y - 1, 0);
			const int iDown = std::min(y + 1, iRows - 1);

			AddConnection(iLeft + y * iCols, i, true);
			AddConnection(iRight + y * iCols, i, true);
			AddConnection(iTop * iCols + x, i, true);
			AddConnection(iDown * iCols + x, i, true);

		}
	}

	void Graph::SetHeuristicFunction(HeuristeicFuncType func)
	{
		m_heuristicFunc = func;
	}

	void Graph::SetBegin(id_type id)
	{
		m_pBegin = GetNode(id);
	}

	void Graph::SetEnd(id_type id)
	{
		m_pEnd = GetNode(id);
	}

	void Graph::Clean()
	{
		m_iRows = 0;
		m_iCols = 0;
		m_nodes.clear();
		m_connections.clear();
		m_pConnections = nullptr;
		m_pBegin = nullptr;
		m_pEnd = nullptr;
	}

	float Graph::GetNodeHeuristic(GraphNode* node)
	{
		return m_heuristicFunc(node, this);
	}

	void Graph::AddConnection(id_type id_from, id_type id_to, bool bBidirectional)
	{
		auto pFrom = GetNode(id_from);
		auto pTo = GetNode(id_to);

		if (pFrom && pTo && pFrom != pTo)
		{
			m_connections[id_from].insert(id_to);
			if (bBidirectional)
				m_connections[id_to].insert(id_from);
		}
	}

}