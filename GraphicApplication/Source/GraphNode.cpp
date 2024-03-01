#include "GraphNode.h"

namespace SaltnPepperEngine
{
#include <algorithm>

	GraphNode::GraphNode(id_type id, Graph* pGraph)
		:
		m_pParent{ nullptr },
		m_Visited{ false },
		m_dwID{ id },
		m_pGraph{ pGraph },
		m_fWeight{ 0 },
		m_Blocked{ false },
		m_fTentativeDistance{ std::numeric_limits<float>::max() }
	{
	}

	GraphNode::~GraphNode()
	{
	}

	void GraphNode::SetParent(GraphNode* parent)
	{
		m_pParent = parent;
	}

	GraphNode* GraphNode::GetParent()
	{
		return m_pParent;
	}

	void GraphNode::SetPosition(const Vector4& pos)
	{
		m_position = pos;
	}

	Vector4 GraphNode::GetPosition()
	{
		return m_position;
	}

	void GraphNode::SetVisited(bool visit)
	{
		m_Visited = visit;
	}

	bool GraphNode::Visited()
	{
		return m_Visited;
	}

	GraphNode::id_type GraphNode::GetID()
	{
		return m_dwID;
	}

	void GraphNode::SetName(std::string name)
	{
		m_name = name;
	}

	std::string GraphNode::GetName()
	{
		return m_name;
	}

	Graph* GraphNode::GetGraph()
	{
		return m_pGraph;
	}

	float GraphNode::GetWeight()
	{
		return m_fWeight;
	}

	float GraphNode::GetTentativeDistance()
	{
		return m_fTentativeDistance;
	}

	bool GraphNode::IsBlocked()
	{
		return m_Blocked;
	}

	void GraphNode::SetGraph(Graph* graph)
	{
		m_pGraph = graph;
	}

	void GraphNode::SetWeight(float fWeight)
	{
		m_fWeight = fWeight;
	}

	void GraphNode::SetBlocked(bool bBlocked)
	{
		m_Blocked = bBlocked;
	}

	void GraphNode::SetTentativeDistance(float fTentativeDistance)
	{
		m_fTentativeDistance = fTentativeDistance;
	}


	

}