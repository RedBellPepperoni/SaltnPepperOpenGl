#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace SaltnPepperEngine
{

	namespace Debug
	{

		std::shared_ptr<spdlog::logger> Log::m_coreLogger;

		std::vector<spdlog::sink_ptr> sinks;


		void Log::OnInit()
		{
			//sinks.emplace_back(std::make_shared<spdlog::sinks::wincolor_stderr_sink_mt>());


			// Create the Logger stuff
			m_coreLogger = spdlog::stderr_color_mt("Console");

			// Configure the Display Pattern (Time Log)
			spdlog::set_pattern("%^[%T] %v%$");

			// Setting level to use all the logging
			m_coreLogger->set_level(spdlog::level::trace);




		}
		void Log::OnRelease()
		{
			m_coreLogger.reset();
			spdlog::shutdown();
		}


		std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
		{
			return m_coreLogger;
		}
		void Log::AddSink(spdlog::sink_ptr& sink)
		{
		}


	}
}