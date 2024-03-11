namespace SW
{
	public static class Log
	{
		/// <summary>
		/// Logs a trace message. Trace messages contain information that is primarily of use to developers or support personnel.
		/// </summary>
		/// <param name="message">The message to be logged. This can be a format string.</param>
		/// <param name="args">Comma-separated list of arguments to format the message.</param>
		public static void Trace(string message, params object[] args)
		{
			unsafe {
				InternalCalls.Log_TraceMessage(string.Format(message, args));
			}
		}

		/// <summary>
		/// Logs an informational message. Informational messages are used to track the general flow of the application and provide useful information.
		/// </summary>
		/// <param name="message">The message to be logged. This can be a format string.</param>
		/// <param name="args">Comma-separated list of arguments to format the message.</param>
		public static void Info(string message, params object[] args) 
		{
			unsafe {
				InternalCalls.Log_InfoMessage(string.Format(message, args));
			}
		}

		/// <summary>
		/// Logs a debug message. Debug messages contain information useful for debugging an application.
		/// </summary>
		/// <param name="message">The message to be logged. This can be a format string.</param>
		/// <param name="args">Comma-separated list of arguments to format the message.</param>
		public static void Debug(string message, params object[] args)
		{
			unsafe {
				InternalCalls.Log_DebugMessage(string.Format(message, args));
			}
		}

		/// <summary>
		/// Logs a warning message. Warning messages are typically for situations that are not errors, but that may need attention during troubleshooting or maintenance.
		/// </summary>
		/// <param name="message">The message to be logged. This can be a format string.</param>
		/// <param name="args">Comma-separated list of arguments to format the message.</param>
		public static void Warn(string message, params object[] args)
		{
			unsafe {
				InternalCalls.Log_WarnMessage(string.Format(message, args));
			}
		}

		/// <summary>
		/// Logs an error message. Error messages are for logging events that are fatal and terminate the program execution.
		/// </summary>
		/// <param name="message">The message to be logged. This can be a format string.</param>
		/// <param name="args">Comma-separated list of arguments to format the message.</param>
		public static void Error(string message, params object[] args)
		{
			unsafe {
				InternalCalls.Log_ErrorMessage(string.Format(message, args));
			}
		}
	}
}
