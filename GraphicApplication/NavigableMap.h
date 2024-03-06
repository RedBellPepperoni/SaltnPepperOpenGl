
#ifndef NAVIGABLEMAP_H
#define NAVIGABLEMAP_H

#include "Engine/Utils/Maths/MathDefinitions.h"

namespace SaltnPepperEngine
{
	namespace AI
	{

		class NavigableMap
		{

		public:

			//GetRandomPoint();

		private:


			Vector2 m_Position;

			Vector2Int m_Bounds;

			Vector2Int m_firstQuadCenter;
			Vector2Int m_secondQuadCenter;
			Vector2Int m_thirdQuadCenter;
			Vector2Int m_forthQuadCenter;





		};

	}
}


#endif // !NAVIGABLEMAP_H


