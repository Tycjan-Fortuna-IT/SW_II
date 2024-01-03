/**
 * @file LogSystem.hpp
 * @author Tycjan Fortuna (242213@edu.p.lodz.pl)
 * @version 0.1.1
 * @date 2024-01-02
 * 
 * @copyright Copyright (c) 2024 Tycjan Fortuna
 */
#pragma once

#include <spdlog/spdlog.h>

namespace SW {

	/**
	 * @brief Enumeration defining different log types.
	 *
	 * This enumeration represents the possible log types that can be used
	 * in the logging system. Each log type has a specific meaning and is used
	 * to categorize log messages.
	 */
	enum LogType : i8
    {
        ENGINE, /**< Log type for engine-related messages. */
        APP,	/**< Log type for application-specific messages. */
    };

	/**
	 * @brief Enumeration defining different log levels.
	 * 
	 * This enumeration represents the possible log levels that can be used
	 * in the logging system. Each log level has a specific meaning and is used
	 * to categorize log messages by their severity.
	 */
    enum LogLevel : i8
    {
        LOG_LEVEL_TRACE, /**< Log level for trace messages. */
        LOG_LEVEL_INFO,  /**< Log level for informational messages. */
        LOG_LEVEL_WARN,  /**< Log level for warning messages. */
        LOG_LEVEL_DEBUG, /**< Log level for debug messages. */
        LOG_LEVEL_ERROR, /**< Log level for error messages. */
        LOG_LEVEL_FATAL, /**< Log level for fatal messages. */
    };

	/**
	 * @brief The LogSystem class is responsible for logging messages to the console.
	 *        It is a static class and should not be instantiated.
	 */
    class LogSystem final
    {
    public:

		/**
		 * @brief Initializes the log system.
		 * 
		 * @warning This method should be called before any logging is done!!
		 */
		static void Initialize();

		/**
		 * @brief Shuts down the log system.
		 * 
		 * @warning This method should be called after all logging is done
		 *			(before application shutdown)!!
		 */
		static void Shutdown();

		/**
		 * @brief Prints using spdlog logger logs to specified sinks.
		 * 		  Type and severity of the log is specified by the parameters.
		 * @note  This method should not be used directly, but rather through the FATAL, ERROR, WARN, INFO, DEBUG, and TRACE macros.
		 * 
		 * @param type The log type to set the log level for.
		 * @param level The log severity level.
		 * @param message The message to log. Must be a fmt formattable string literal. e.g. "Hello, {}!"
		 * @param args The arguments to the message to be formatted.
		 */
		template <typename... Args>
        static void PrintMessage(LogType type, LogLevel level, const std::string& message, Args&&... args) {
			const std::shared_ptr<spdlog::logger>& logger = type == LogType::ENGINE ? s_EngineLogger : s_AppLogger;

			switch (level) {
				case LOG_LEVEL_TRACE:
					logger->trace(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...))); break;
				case LOG_LEVEL_INFO:
					logger->info(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...))); break;
				case LOG_LEVEL_WARN:
					logger->warn(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...))); break;
				case LOG_LEVEL_DEBUG:
					logger->debug(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...))); break;
				case LOG_LEVEL_ERROR:
					logger->error(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...))); break;
				case LOG_LEVEL_FATAL:
					logger->critical(fmt::vformat(message, fmt::make_format_args(std::forward<Args>(args)...)));
					__debugbreak(); break;
			}
		}

		/**
		 * @brief Reports an assertion failure.
		 * @note  This method should not be used directly, but rather through the ASSERT macro.
		 * 
		 * @param expression String representation of the expression that failed.
		 * @param message The message to log.
		 * @param file File in which the assertion failed.
		 * @param line Line at which the assertion failed.
		 */
        static void ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line);

    private:
		/**
		 * @brief Logger for engine related logs.
		 */
		static std::shared_ptr<spdlog::logger> s_EngineLogger;

		/**
		 * @brief Logger for client's application related logs.
		 */
		static std::shared_ptr<spdlog::logger> s_AppLogger;
    };

    /**
     * @brief Asserts that an expression is true. (Engine use only.)
     *        If the expression is false, the application will be halted.
     * @param x The expression to assert.
     * @param msg The message to print if the assertion fails.
     */
    #define ASSERT(x, msg)															 \
    {																				 \
        if (!(x))																	 \
        {																			 \
            ::SW::LogSystem::ReportAssertionFailure(#x, msg, __FILE__, __LINE__);    \
            __debugbreak();															 \
        }																			 \
    }

    /**
     * @brief Logs a message.
     * @note This macro should not be used directly, but rather through the FATAL, ERROR, WARN, INFO, DEBUG, and TRACE macros.
     *
     * @param level The level of the message.
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_LOG(type, level, msg, ...) ::SW::LogSystem::PrintMessage(type, level, msg, ##__VA_ARGS__)

    /**
     * @brief Logs a fatal message.
     *        This macro should be used to stop the application when hit.
     * @note Engine use only.
	 * 
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_FATAL(msg, ...) { SW_LOG(ENGINE, LOG_LEVEL_FATAL, msg, ##__VA_ARGS__); __debugbreak(); }

    /**
     * @brief Logs an error message.
     *        This macro should be used to indicate critical runtime problems that cause the application to run improperly or not at all.
     * @note Engine use only.
	 * 
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_ERROR(msg, ...) SW_LOG(ENGINE, LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

    /**
     * @brief Logs a warning message.
     *        This macro should be used to indicate non-critial problems with the application that cause it to run suboptimally.
     * @note Engine use only.
	 * 
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_WARN(msg, ...) SW_LOG(ENGINE, LOG_LEVEL_WARN, msg, ##__VA_ARGS__)

    /**
     * @brief Logs an info message.
     *        This macro should be used for non-erronuous informational purposes.
	 * @note Engine use only.
     *
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_INFO(msg, ...) SW_LOG(ENGINE, LOG_LEVEL_INFO, msg, ##__VA_ARGS__)

    /**
     * @brief Logs a debug message.
     *        This macro should be used for debugging purposes.
	 * @note Engine use only.
     *
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_DEBUG(msg, ...) SW_LOG(ENGINE, LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)

    /**
     * @brief Logs a trace message.
     *        This macro should be used for verbose debugging purposes.
     * @note Engine use only.
	 * 
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define SW_TRACE(msg, ...) SW_LOG(ENGINE, LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)

	/**
	 * @brief Logs a fatal message.
	 * 	  	  This macro should be used to stop the application when hit.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_FATAL(msg, ...) { SW_LOG(::SW::APP, ::SW::LOG_LEVEL_FATAL, msg, ##__VA_ARGS__); __debugbreak(); }

	/**
	 * @brief Logs an error message.
	 * 	  	  This macro should be used to indicate critical runtime problems that cause the application to run improperly or not at all.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_ERROR(msg, ...) SW_LOG(::SW::APP, ::SW::LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

	/**
	 * @brief Logs a warning message.
	 * 	  	  This macro should be used to indicate non-critial problems with the application that cause it to run suboptimally.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_WARN(msg, ...) SW_LOG(::SW::APP, ::SW::LOG_LEVEL_WARN, msg, ##__VA_ARGS__)

	/**
	 * @brief Logs an info message.
	 * 	  	  This macro should be used for non-erronuous informational purposes.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_INFO(msg, ...) SW_LOG(::SW::APP, ::SW::LOG_LEVEL_INFO, msg, ##__VA_ARGS__)

	/**
	 * @brief Logs a debug message.
	 * 	  	  This macro should be used for debugging purposes.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_DEBUG(msg, ...) SW_LOG(::SW::APP, ::SW::LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)

	/**
	 * @brief Logs a trace message.
	 * 	  	  This macro should be used for verbose debugging purposes.
	 * @note Application use only.
	 * 
	 * @param msg The message to log.
	 * @param ... The arguments to the message to be formatted.
	 */
	#define APP_TRACE(msg, ...) SW_LOG(::SW::APP, ::SW::LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
}
