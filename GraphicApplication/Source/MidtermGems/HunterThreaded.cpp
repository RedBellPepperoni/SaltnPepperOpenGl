#include "HunterThreaded.h"
#include "SaltnPepperEngine.h"
namespace SaltnPepperEngine
{

    DWORD WINAPI UpdateThreadHunter(LPVOID lpParameter)
    {
        HunterThreadInfo* threadInfo = (HunterThreadInfo*)lpParameter;

        DWORD sleepTime_ms = 1;

        double lastFrameTime = glfwGetTime();
        double totalElapsedFrameTime = 0.0;

        while (threadInfo->isAlive)
        {
            if (threadInfo->run)
            {
                // Adjust sleep time based on actual framerate
                //float delta = Time::DeltaTime();

                double currentTime = glfwGetTime();
                float deltaTime = (float)(currentTime - lastFrameTime);
                lastFrameTime = currentTime;
              
                totalElapsedFrameTime += deltaTime;

                threadInfo->hunterRef->Update(deltaTime);

                // Maybe something small like 1ms delay?
                Sleep(1);

            }//if ( pSBTInfo->bRun )
        }//while (pSBTInfo->bIsAlive)

        return 0;
    }
}