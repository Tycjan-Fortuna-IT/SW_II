#include "LogSystem.hpp"

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace SW {

	std::shared_ptr<spdlog::logger> LogSystem::s_EngineLogger;
	std::shared_ptr<spdlog::logger> LogSystem::s_AppLogger;

	/**
	 * @brief ANSI escape code for black text.
	 */
	#define ANSI_COLOR_BLACK   "\x1b[30m"

	/**
	 * @brief ANSI escape code for red text.
	 */
	#define ANSI_COLOR_RED     "\x1b[31m"

	/**
	 * @brief ANSI escape code for green text.
	 */
	#define ANSI_COLOR_GREEN   "\x1b[32m"

	/**
	 * @brief ANSI escape code for yellow text.
	 */
	#define ANSI_COLOR_YELLOW  "\x1b[33m"

	/**
	 * @brief ANSI escape code for blue text.
	 */
	#define ANSI_COLOR_BLUE    "\x1b[34m"

	/**
	 * @brief ANSI escape code for magenta text.
	 */
	#define ANSI_COLOR_MAGENTA "\x1b[35m"

	/**
	 * @brief ANSI escape code for cyan text.
	 */
	#define ANSI_COLOR_CYAN    "\x1b[36m"

	/**
	 * @brief ANSI escape code for white text.
	 */
	#define ANSI_COLOR_WHITE   "\x1b[37m"

	/**
	 * @brief ANSI escape code for black background.
	 */
	#define ANSI_BG_COLOR_BLACK   "\x1b[40m"

	/**
	 * @brief ANSI escape code for red background.
	 */
	#define ANSI_BG_COLOR_RED     "\x1b[41m"

	/**
	 * @brief ANSI escape code for green background.
	 */
	#define ANSI_BG_COLOR_GREEN   "\x1b[42m"

	/**
	 * @brief ANSI escape code for yellow background.
	 */
	#define ANSI_BG_COLOR_YELLOW  "\x1b[43m"

	/**
	 * @brief ANSI escape code for blue background.
	 */
	#define ANSI_BG_COLOR_BLUE    "\x1b[44m"

	/**
	 * @brief ANSI escape code for magenta background.
	 */
	#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"

	/**
	 * @brief ANSI escape code for cyan background.
	 */
	#define ANSI_BG_COLOR_CYAN    "\x1b[46m"

	/**
	 * @brief ANSI escape code for white background.
	 */
	#define ANSI_BG_COLOR_WHITE   "\x1b[47m"

	/**
	 * @brief ANSI escape code to reset text and background color.
	 */
	#define ANSI_COLOR_RESET   "\x1b[0m"

	void LogSystem::Initialize()
	{
		std::vector<spdlog::sink_ptr> logSinks;

		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/SW.log", 0, 0));

		logSinks[0]->set_pattern("%^[%T] [%n] [%l]: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] [%n] [%l]: %v");

		s_EngineLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(s_EngineLogger);

		s_AppLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		s_AppLogger->set_level(spdlog::level::trace);
		s_AppLogger->flush_on(spdlog::level::trace);
		spdlog::register_logger(s_AppLogger);
	}

	void LogSystem::Shutdown()
	{
		s_EngineLogger.reset();
		s_AppLogger.reset();
		spdlog::drop_all();
	}

    void LogSystem::ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line)
	{
		LogSystem::PrintMessage(LogType::ENGINE, LogLevel::LOG_LEVEL_FATAL, "Assertion Failure: {}, message: '{}', in file: {}, line: {}\n", expression, message, file, line);
    }

}
