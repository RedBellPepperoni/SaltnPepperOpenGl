#include "DungeonManager.h"
#include "Engine/Utils/Loading/FileSystem.h"
#include "Engine/Utils/Logging/Log.h"
#include <sstream>
#include <string>


namespace SaltnPepperEngine
{
	bool DungeonManager::LoadTSVMap(const std::string& filepath)
	{
        std::string tempFile = FileSystem::ReadFileToString(filepath);

        //file is not loaded, or wmpty
        if (tempFile.empty()) {return false;}

        // Initializing the string stream with the loaded Text data
        std::istringstream stringStream(tempFile);

        std::string key;
        int rows = 0;
     
        // Read line by line to find the required Data
        do
        {
            stringStream >> key;
          
            if (key == "DUNGEON_BEGIN") {continue;}

            // End of header data
            if (key == "DUNGEON_END") { break; }
               
            bool wall = false;
            int columns = key.size();

            for (int i = 0; i < columns; i++)
            {
                char data = key[i];

                if (data == 'X')  { wall = true;}
                else if (data == '.') { wall = false; }

                m_wallData.push_back(wall);
            }

             rows++;  

        } while (stringStream);

        return true;

	}


    void DungeonManager::SetStartandEnd()
    {
        const size_t wallsSize = m_wallData.size();

        for (int i = 0; i < wallsSize; i++)
        {
            // Find the first non wall
            if (m_wallData[i] == false)
            {
                startIndex = i;
                break;
            }
        }

        for (int i = wallsSize-1; i > 0; i--)
        {
            // Find the last non wall
            if (m_wallData[i] == false)
            {
                endIndex = i;
                break;
            }
        }


    }

    void DungeonManager::Create3DMap()
    {
        //Fixed Coords for now
        for (int y = 0; y < 149; y++)
        {
            for (int x = 0; x < 115; x++)
            {
                int index = x + y * 115;

                Vector3 position = Vector3(x, 0.0f, y);

                if (m_wallData[index])
                {
                    CreateWall(position);
                }
                else
                {
                    CreateFloor(position);
                }

            }

           
        }
    }
}