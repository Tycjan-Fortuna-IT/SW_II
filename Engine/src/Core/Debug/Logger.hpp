#pragma once

namespace SW {

    enum LogLevel : i8
    {
        LOG_LEVEL_FATAL,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_TRACE,
    };

    class Logger final
    {
    public:
        static void PrintMessage(LogLevel level, const char* message, ...);
        static void ReportAssertionFailure(const char* expression, const char* message, const char* file, i16 line);
    };

    /**
     * @brief Asserts that an expression is true.
     *        If the expression is false, the application will be halted.
     * @param x The expression to assert.
     * @param msg The message to print if the assertion fails.
     */
    #define ASSERT(x, msg)                                                  \
    {                                                                       \
        if (!(x))                                                           \
        {                                                                   \
            Logger::ReportAssertionFailure(#x, msg, __FILE__, __LINE__);    \
            __debugbreak();                                                 \
        }                                                                   \
    }

    /**
     * @brief Logs a message.
     *        This macro should not be used directly, but rather through the FATAL, ERROR, WARN, INFO, DEBUG, and TRACE macros.
     *
     * @param level The level of the message.
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define LOG(level, msg, ...) Logger::PrintMessage(level, msg, ##__VA_ARGS__)

    /**
     * @brief Logs a fatal message.
     *        This macro should be used to stop the application when hit.
     *
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define FATAL(msg, ...) { LOG(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__); __debugbreak(); }

    /**
     * @brief Logs an error message.
     *        This macro should be used to indicate critical runtime problems that cause the application to run improperly or not at all.
     *
     * @param msg The message to log.
     * @param ... The arguments to the message to be formatted.
     */
    #define ERROR(msg, ...) LOG(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)

     /**
      * @brief Logs a warning message.
      *        This macro should be used to indicate non-critial problems with the application that cause it to run suboptimally.
      *
      * @param msg The message to log.
      * @param ... The arguments to the message to be formatted.
      */
    #define WARN(msg, ...) LOG(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)

     /**
      * @brief Logs an info message.
      *        This macro should be used for non-erronuous informational purposes.
      *
      * @param msg The message to log.
      * @param ... The arguments to the message to be formatted.
      */
    #define INFO(msg, ...) LOG(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)

     /**
      * @brief Logs a debug message.
      *        This macro should be used for debugging purposes.
      *
      * @param msg The message to log.
      * @param ... The arguments to the message to be formatted.
      */
    #define DEBUG(msg, ...) LOG(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)

     /**
      * @brief Logs a trace message.
      *        This macro should be used for verbose debugging purposes.
      *
      * @param msg The message to log.
      * @param ... The arguments to the message to be formatted.
      */
    #define TRACE(msg, ...) LOG(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)

}
