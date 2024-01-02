#include "Logger.hpp"

#include <cstdarg>
#include <cstdio>

#include <spdlog/spdlog.h>

namespace SW {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;

    /*
    * @brief Macros to change the color of the text in the terminal
    */
    #define ANSI_COLOR_BLACK   "\x1b[30m"
    #define ANSI_COLOR_RED     "\x1b[31m"
    #define ANSI_COLOR_GREEN   "\x1b[32m"
    #define ANSI_COLOR_YELLOW  "\x1b[33m"
    #define ANSI_COLOR_BLUE    "\x1b[34m"
    #define ANSI_COLOR_MAGENTA "\x1b[35m"
    #define ANSI_COLOR_CYAN    "\x1b[36m"
    #define ANSI_COLOR_WHITE   "\x1b[37m"

    /*
    * @brief Macros to change the background color of the text in the terminal
    */
    #define ANSI_BG_COLOR_BLACK   "\x1b[40m"
    #define ANSI_BG_COLOR_RED     "\x1b[41m"
    #define ANSI_BG_COLOR_GREEN   "\x1b[42m"
    #define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
    #define ANSI_BG_COLOR_BLUE    "\x1b[44m"
    #define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
    #define ANSI_BG_COLOR_CYAN    "\x1b[46m"
    #define ANSI_BG_COLOR_WHITE   "\x1b[47m"

    /*
    * @brief Macro to reset the colors in the terminal
    */
    #define ANSI_COLOR_RESET   "\x1b[0m"

    void Logger::PrintMessage(LogType type, LogLevel level, const char* message, ...) {
        static const char* log_types[2] = { "[ENGINE]", "[ APP  ]" };
        static const char* level_strings[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: " };
        static const char* level_colors[6] = { ANSI_COLOR_RED, ANSI_COLOR_RED, ANSI_COLOR_YELLOW, ANSI_COLOR_BLUE, ANSI_COLOR_MAGENTA, ANSI_COLOR_GREEN };

        va_list args;
        va_start(args, message);

        printf("%s%s%s%s", log_types[type], level_colors[level], level_strings[level], ANSI_COLOR_RESET);
        vprintf(message, args);
        printf("\n");

        va_end(args);
    }

    void Logger::ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line) {
        Logger::PrintMessage(LogType::ENGINE, LogLevel::LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expression, message, file, line);
    }

}
