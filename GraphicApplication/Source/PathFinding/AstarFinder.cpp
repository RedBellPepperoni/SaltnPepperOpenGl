#include "AstarFinder.h"
#include "Graph.h"


namespace SaltnPepperEngine
{
	AstarFinder::AstarFinder()
	{
	}
	AstarFinder::~AstarFinder()
	{

	}

	bool AstarFinder::Step()
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
	void AstarFinder::LoadConnections(GraphNode* pNode)
	{
		auto connections = std::move(GetConnections(pNode));

		for (auto& node : connections)
		{
			float fTentDist = pNode->GetTentativeDistance() + node->GetWeight();
			float fNodeTent = node->GetTentativeDistance();
			if (!node->Visited() && !node->IsBlocked() &&
				fTentDist < fNodeTent)
			{
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
	void AstarFinder::ClearNodes()
	{
		m_nodes.clear();
	}
	bool LessF::operator()(GraphNode* pA, GraphNode* pB) const
	{
		Graph* pGraph = pA->GetGraph();
		float hA = pGraph->GetNodeHeuristic(pA);
		float hB = pGraph->GetNodeHeuristic(pB);
		return (hA + pA->GetTentativeDistance()) < (hB + pB->GetTentativeDistance());
	}
}