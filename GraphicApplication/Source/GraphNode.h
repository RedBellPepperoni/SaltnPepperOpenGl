#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <vector>
#include <string>
#include "Engine/Utils/Maths/MathDefinitions.h"


namespace SaltnPepperEngine
{
	class Graph;

	class GraphNode
	{

	public:
		using id_type = unsigned int;
		GraphNode(id_type id, Graph* pGraph);
		GraphNode(const GraphNode& node) = default;
		~GraphNode();

		GraphNode& operator=(const GraphNode& node) = default;
		bool Visited();
		void SetParent(GraphNode* parent);
		void SetVisited(bool visit);
		void SetPosition(const Vector4& pos);
		void SetName(std::string name);
		void SetGraph(Graph* graph);
		void SetWeight(float fWeight);
		void SetBlocked(bool bBlocked);
		void SetTentativeDistance(float fTentativeDistance);

		Vector4 GetPosition();

		GraphNode* GetParent();
		id_type GetID();
		std::string GetName();
		Graph* GetGraph();
		float GetWeight();
		float GetTentativeDistance();
		bool IsBlocked();
	
	private:
		id_type m_dwID;
		float m_fTentativeDistance;
		float m_fWeight;
		bool m_Visited;
		bool m_Blocked;
	    Graph* m_pGraph;
		std::string m_name;
		GraphNode* m_pParent;
		Vector4 m_position;

	};


}

#endif // !GRAPH_NODE_H

