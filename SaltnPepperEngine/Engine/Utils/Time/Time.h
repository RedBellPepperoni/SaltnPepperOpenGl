#ifndef TIME_H
#define TIME_H

#include <cstdint>
#include <cstddef>


namespace SaltnPepperEngine
{
	using TimeStep = float;


	class Time
	{

	public:

		// gets the Currenttime in Milliseconds
		static TimeStep CurrentTime();

		// the Current time since the start of the engine
		static TimeStep CurrentEngineTime();

		// Sets the current time
		static void SetCurrentTime(TimeStep time);

		// Get the operating system time
		static int64_t SystemTime();

		// gets the time that has passed between frames
		static TimeStep DeltaTime();

		// Getst he unscaled time sicne last frame
		static TimeStep UnscaledDeltaTime();

		// gets the average Frames per seconds
		static size_t GetFPS();

		// Sleeps teh current thread for the given amount of seconds
		static void Sleep(float seconds);


	};

}

#endif

