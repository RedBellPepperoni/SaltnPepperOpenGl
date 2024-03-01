#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include "TileDefinitions.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <unordered_map>


namespace SaltnPepperEngine
{



	class DungeonManager
	{
	public:

		DungeonManager(); 
		~DungeonManager();


	private:

		void LoadFromFile();

		std::vector<bool> m_wallData;
		int m_rows = 0;
		int m_cols = 0;

		float m_nodeWidth = 0.0f;
		float m_nodeHeight = 0.0f;

	};

	

}

#endif // !DUNGEONMANAGER_H
