#ifndef ASTARFINDER_H
#define ASTARFINDER_H
#include "PathFinder.h"

namespace SaltnPepperEngine
{

	class LessF
	{
	public:
		bool operator()(GraphNode* pA, GraphNode* pB) const;
	};


	class AstarFinder : public PathFinder
	{
	public:
		AstarFinder();
		virtual ~AstarFinder();
		bool Step() override;
	private:
		void LoadConnections(GraphNode* pNode) override;
		void ClearNodes() override;
	private:
		std::multiset<GraphNode*, LessF> m_nodes;
	};


}

#endif // !ASTARFINDER_H

