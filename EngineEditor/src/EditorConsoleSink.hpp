#pragma once

#include "Panels/ConsolePanel.hpp"
#include "spdlog/sinks/base_sink.h"

namespace SW
{
	static constexpr int MAX_MESSAGES = 10;

	template <typename Mutex>
	class EditorConsoleSink final : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		EditorConsoleSink(ConsolePanel* console) : m_ConsolePanel(console) {}

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			// TO DO investigate why std::string(formatted.data(), formatted.size()) has to be used
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

			Logger::LogLevel logLevel = [&msg]() -> Logger::LogLevel {
				if (msg.level == spdlog::level::trace)
					return Logger::LogLevel::LOG_LEVEL_TRACE;
				else if (msg.level == spdlog::level::debug)
					return Logger::LogLevel::LOG_LEVEL_DEBUG;
				else if (msg.level == spdlog::level::info)
					return Logger::LogLevel::LOG_LEVEL_INFO;
				else if (msg.level == spdlog::level::warn)
					return Logger::LogLevel::LOG_LEVEL_WARN;
				else if (msg.level == spdlog::level::err)
					return Logger::LogLevel::LOG_LEVEL_ERROR;
				else if (msg.level == spdlog::level::critical)
					return Logger::LogLevel::LOG_LEVEL_FATAL;

				return Logger::LogLevel::LOG_LEVEL_TRACE;
			}();

			m_Messages[(size_t)m_MessageCount++] = {.Level   = logLevel,
			                                        .Message = std::string(formatted.data(), formatted.size()),
			                                        .Time    = std::chrono::system_clock::to_time_t(msg.time)};
		}

		void flush_() override
		{
			for (int i = 0; i < m_MessageCount; i++)
			{
				m_ConsolePanel->AddMessage(m_Messages[(size_t)i]);
			}

			m_MessageCount = 0;
		}

	private:
		ConsolePanel* m_ConsolePanel = nullptr;

		std::array<ConsoleMessage, MAX_MESSAGES> m_Messages;
		int m_MessageCount = 0;
	};

} // namespace SW
