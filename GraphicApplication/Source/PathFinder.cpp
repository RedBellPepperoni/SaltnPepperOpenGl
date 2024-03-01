#include "PathFinder.h"
#include "GraphNode.h"
#include "Graph.h"
namespace SaltnPepperEngine
{

	PathFinder::PathFinder() :
		m_iMaxSteps{ 10000 },
		m_pCurrent{ nullptr },
		m_iCurrentSteps{ 0 },
		m_pEnd{ nullptr },
		m_iBegin{ static_cast<id_type>(-1) },
		m_iEnd{ static_cast<id_type>(-1) }
	{
	}

	PathFinder::~PathFinder()
	{
	}

	void PathFinder::FindPath(const std::shared_ptr<Graph>& pGraph)
	{
		Setup(pGraph);
		while (!Step());
		Cleanup();
	}

	GraphNode* PathFinder::GetCurrentNode()
	{
		return m_pCurrent;
	}

	std::deque<GraphNode*> PathFinder::GetPath()
	{
		return m_path;
	}

	void PathFinder::Setup(const SharedPtr<Graph>& pGraph)
	{
		m_pGraph = pGraph;

		m_pGraph->SetBegin(m_iBegin);
		m_pGraph->SetEnd(m_iEnd);

		m_iCurrentSteps = 0;

		m_pCurrent = m_pGraph->GetBeginNode();
		m_pEnd = m_pGraph->GetEndNode();

		if (m_pCurrent)
		{
			m_pCurrent->SetVisited(true);
			m_pCurrent->SetTentativeDistance(m_pCurrent->GetWeight());
			LoadConnections(m_pCurrent);
		}

		ClearPath();
	}

	bool PathFinder::Step()
	{
		if (m_pEnd && m_pCurrent &&
			m_iCurrentSteps++ < m_iMaxSteps && !m_nodes.empty())
		{
			m_pCurrent = *m_nodes.begin();

			m_pCurrent->SetVisited(true);

			m_nodes.erase(m_nodes.begin());

			if (m_pCurrent == m_pEnd)
			{
				CreatePath(m_pCurrent);
				return true;
			}

			LoadConnections(m_pCurrent);

		}
		else
		{
			//CreatePath(m_pCurrent);
			return true;
		}

		return false;
	}

	void PathFinder::Cleanup()
	{
		ClearNodes();
		m_pGraph = nullptr;
		m_pCurrent = nullptr;
		m_pEnd = nullptr;
	}

	void PathFinder::SetMaxSteps(int iMaxSteps)
	{
		m_iMaxSteps = iMaxSteps;
	}

	void PathFinder::SetBegin(id_type iBegin)
	{
		m_iBegin = iBegin;
	}

	void PathFinder::SetEnd(id_type iEnd)
	{
		m_iEnd = iEnd;
	}

	void PathFinder::ClearPath()
	{
		m_path.clear();
	}

	PathFinder::id_type PathFinder::GetBegin()
	{
		return m_iBegin;
	}

	PathFinder::id_type PathFinder::GetEnd()
	{
		return m_iEnd;
	}

	void PathFinder::LoadConnections(GraphNode* pNode)
	{
		std::vector<GraphNode*> connections = std::move(GetConnections(pNode));

		for (GraphNode*& node : connections)
		{
			float fTentDist = pNode->GetTentativeDistance() + node->GetWeight();
			float fNodeTent = node->GetTentativeDistance();
			if (!node->Visited() && !node->IsBlocked() &&
				fTentDist < fNodeTent)
			{
				// tree Iterator
				auto it = m_nodes.find(node);

				if (it != m_nodes.end())
				{
					m_nodes.erase(it);
				}

				node->SetTentativeDistance(fTentDist);

				m_nodes.insert(node);

				node->SetParent(pNode);
			}
		}
	}

	void PathFinder::ClearNodes()
	{
		m_nodes.clear();
	}

	void PathFinder::CreatePath(GraphNode* pEnd)
	{
		m_path.clear();

		while (pEnd)
		{
			m_path.push_front(m_pGraph->GetNode(pEnd->GetID()));
			pEnd = pEnd->GetParent();
		}

	}


	std::vector<GraphNode*> PathFinder::GetConnections(GraphNode* pNode)
	{
		return  m_pGraph->GetNodeConnections(pNode);
	}
	bool LessThanF::operator()(GraphNode* pA, GraphNode* pB) const
	{
		Graph* pGraph = pA->GetGraph();
		float hA = pGraph->GetNodeHeuristic(pA);
		float hB = pGraph->GetNodeHeuristic(pB);
		return (hA + pA->GetTentativeDistance()) < (hB + pB->GetTentativeDistance());
	}
}