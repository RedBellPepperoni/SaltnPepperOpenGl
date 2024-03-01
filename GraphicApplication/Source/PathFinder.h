#ifndef  PATHFINDER_H
#define  PATHFINDER_H
#include "TileDefinitions.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <vector>

namespace SaltnPepperEngine
{

	

	class PathFinder
	{

	public:

		PathFinder();
		~PathFinder() = default;




		std::vector<UniquePtr<Tile>> tiles;

	private:

		int m_Weight = 1;
		int m_YMax = 0;
		int m_XMax = 0;

		float moveCost = 0.0f;

		bool Dijkstra(int x, int y);
		void DijkstraAddNode();

		SharedPtr<Tile> GetTile(int x, int y);
			 
	};




}

#endif // ! PATHFINDER_H
