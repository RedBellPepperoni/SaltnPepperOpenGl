#ifndef LOG_H
#define LOG_H

#include "spdlog/spdlog.h"
#include <spdlog/fmt/ostr.h>

#define LOG_TRACE(...) ::SaltnPepperEngine::Debug::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::SaltnPepperEngine::Debug::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::SaltnPepperEngine::Debug::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::SaltnPepperEngine::Debug::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::SaltnPepperEngine::Debug::Log::GetCoreLogger()->critical(__VA_ARGS__)



namespace SaltnPepperEngine
{


	namespace Debug
	{

		class Log
		{

		public:

			static void OnInit();
			static void OnRelease();

			static std::shared_ptr <spdlog::logger>& GetCoreLogger();

			static void AddSink(spdlog::sink_ptr& sink);
		private:

			static std::shared_ptr <spdlog::logger> m_coreLogger;
		};

	}
}




#endif // !LOG_H


