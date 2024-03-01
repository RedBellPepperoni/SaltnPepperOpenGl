#ifndef GRAPH_H
#define GRAPH_H
#include "GraphNode.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>


namespace SaltnPepperEngine
{
	class Graph
	{
	public:
		using id_type = unsigned int;
		using ConnectionsMap = std::unordered_map<id_type, std::unordered_set<id_type>>;
		using NodesMap = std::vector<UniquePtr<GraphNode>>;
		using HeuristeicFuncType = std::function<float(GraphNode* node, Graph* graph)>;
	public:
		Graph();
		Graph(const Graph& graph);
		Graph& operator=(const Graph& graph);
		~Graph();
		GraphNode* GetNode(id_type id);
		GraphNode* GetBeginNode();
		GraphNode* GetEndNode();
		ConnectionsMap GetConnections();
		std::vector<GraphNode*> GetNodeConnections(GraphNode* node);
		std::vector<GraphNode*> GetNodes();
		void Create(int iCols, int iRows, float fNodeWidth, float fNodeHeight);
		void Create(const std::vector<bool>& mapdata, int iCols, int iRows, float fNodeWidth, float fNodeHeight);

		void SetHeuristicFunction(HeuristeicFuncType func);
		void SetBegin(id_type id);
		void SetEnd(id_type id);
		void Clean();
		float GetNodeHeuristic(GraphNode* node);
	private:
		void AddConnection(id_type id_from, id_type id_to, bool bBidirectional);
	private:
		HeuristeicFuncType m_heuristicFunc;
		int m_iRows;
		int m_iCols;
		float m_fNodeWidth;
		float m_fNodeHeight;
		NodesMap m_nodes;
		ConnectionsMap m_connections;
		ConnectionsMap* m_pConnections;
		GraphNode* m_pBegin;
		GraphNode* m_pEnd;

	};


}

#endif // !GRAPH_H

