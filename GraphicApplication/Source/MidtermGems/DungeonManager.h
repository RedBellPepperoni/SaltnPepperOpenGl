#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H
#include "EntitySetup.h"
#include "Engine/Utils/Maths/MathDefinitions.h"
#include <string>
#include <vector>


namespace SaltnPepperEngine
{

	class DungeonManager
	{
	public:

		DungeonManager() = default; 
		~DungeonManager() = default;

		bool LoadTSVMap(const std::string& filepath);
		void SetStartandEnd();
		void Create3DMap();
		const std::vector<bool>& GetWallData() const{ return m_wallData; }

	private:

		std::vector<bool> m_wallData;
		std::vector<std::string> m_printableData;
		int m_rows = 150;
		int m_cols = 116;

		int startIndex = -1;
		int endIndex = -1;

		float m_nodeWidth = 0.0f;
		float m_nodeHeight = 0.0f;

	};


}

#endif // !DUNGEONMANAGER_H
