#ifndef  PATHFINDER_H
#define  PATHFINDER_H
#include <deque>
#include <memory>
#include <vector>
#include <set>
#include "Engine/Core/Memory/MemoryDefinitions.h"


namespace SaltnPepperEngine
{

	class GraphNode;
	class Graph;

	class LessThanF
	{
	public:
		bool operator()(GraphNode* pA, GraphNode* pB) const;
	};

	class PathFinder
	{

	public:
		using id_type = unsigned int;
		PathFinder();
		virtual ~PathFinder();
		//Find the path in 1 call
		virtual void FindPath(const SharedPtr<Graph>& pGraph);
		//Find the path by steps
		/**********************/
		virtual void Setup(const SharedPtr<Graph>& pGraph);
		virtual bool Step();
		virtual void Cleanup();
		/**********************/
		//Max number of iterations before cancelling the search
		void SetMaxSteps(int iMaxSteps);
		void SetBegin(id_type iBegin);
		void SetEnd(id_type iEnd);
		void ClearPath();
		id_type GetBegin();
		id_type GetEnd();
		//Used externally when finding the path by steps
		GraphNode* GetCurrentNode();
		std::deque<GraphNode*> GetPath();
			 
	protected:
		virtual void LoadConnections(GraphNode* pNode);
		virtual void ClearNodes();
		void CreatePath(GraphNode* pEnd);
		std::vector<GraphNode*> GetConnections(GraphNode* pNode);
	protected:
		std::shared_ptr<Graph> m_pGraph;
		GraphNode* m_pCurrent;
		GraphNode* m_pEnd;
		int m_iMaxSteps;
		int m_iCurrentSteps;
		id_type m_iBegin;
		id_type m_iEnd;
	private:
		std::deque<GraphNode*> m_path;
		std::multiset<GraphNode*, LessThanF> m_nodes;
	};




}

#endif // ! PATHFINDER_H
