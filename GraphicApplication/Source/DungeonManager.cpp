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

        if (tempFile.empty())
        {
            // Model not loaded
            return false;
        }

     
         // Initializing the string stream with the loaded Text data
        std::istringstream stringStream(tempFile);

       // bool correctDungeon = false;

        std::string key;
        int rows = 0;
        
        //m_wallData.resize(115*149,false);
  
        // Read line by line to find the number of vertices
        do
        {
            stringStream >> key;


          
          if (key == "DUNGEON_BEGIN")
          {
              continue;
          }

              

          if (key == "DUNGEON_END")
          {
               // End of header data, now the actual vertex and index data starts
               break;
          }


          bool wall = false;
          int columns = key.size();



          

          for (int i = 0; i < columns; i++)
          {

              char data = key[i];

              if (data == 'X')
              {
                  wall = true;
              }

              else if (data == '.')
              {
                  wall = false;
              }

              //m_wallData[rows + i]= (wall);
              m_wallData.push_back(wall);


          }

          rows++;

      
              

            

        } while (stringStream);

        m_wallData;

        rows;
        // Assuming there is only one mesh per file (for now)

        // =============================== Loading Vertex data from the file ==================================
    
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


    void DungeonManager::PrintMap()
    {
       /* for (int y = 0; y < 149; y++)
        {
            for (int x = 0; x < 115; x++)
            {
                int index = x + y * 115;
                if (m_wallData[index])
                {
                    std::cout << "X";
                }
                else
                {
                    std::cout << ".";
                }

            }

            std::cout << std::endl;
        }*/

        //std::cout << m_printableData[116] << std::endl;
    }
    void DungeonManager::Create3DMap()
    {
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