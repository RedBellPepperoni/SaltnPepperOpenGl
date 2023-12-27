#include "Time.h"
#include <chrono>
#include <thread>
#include "Engine/Core/System/Application/Application.h"

namespace SaltnPepperEngine
{
	TimeStep Time::CurrentTime()
	{
		return Application::GetCurrent().GetTotalElapsed();
	}

	TimeStep Time::CurrentEngineTime()
	{
		return Application::GetCurrent().GetGLFWTime();
	}

	void Time::SetCurrentTime(TimeStep time)
	{
		Application::GetCurrent().SetTotalElapsed(time);
	}

	// use very sparing ly, high performance impact
	int64_t Time::SystemTime()
	{
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}


	TimeStep Time::DeltaTime()
	{
		return Application::GetCurrent().GetDelta();
	}
	TimeStep Time::UnscaledDeltaTime()
	{
		return  Application::GetCurrent().GetUnscaledDelta();
	}
	size_t Time::GetFPS()
	{
		return Application::GetCurrent().GetFPS();
	}

	void Time::Sleep(float seconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(uint64_t(seconds * 1000.0f)));
	}


}

