#ifndef HUNTERTHREADED_H
#define HUNTERTHREADED_H
#include "TreasureHunter.h"
#include "Engine/Core/Memory/MemoryDefinitions.h"
#include <Windows.h>	
#include "Engine/Utils/Time/Time.h"

#define WIN32_LEAN_AND_MEAN	   

namespace SaltnPepperEngine
{
	struct HunterThreadInfo
	{
		SharedPtr<TreasureHunter> hunterRef = nullptr;
		//TreasureHunter* hunterRef;
		float deltatime = 0.0f;

		bool run = false;
		bool isAlive = false;
		DWORD sleepTime;

	};

	DWORD WINAPI UpdateThreadHunter(LPVOID lpParameter);
  


	


}

#endif // !HUNTERTHREADED_H


