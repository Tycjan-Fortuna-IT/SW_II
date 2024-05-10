#pragma once

#include "spdlog/sinks/base_sink.h"
#include "Panels/ConsolePanel.hpp"

namespace SW {

	static constexpr int MAX_MESSAGES = 10;

	template<typename Mutex>
	class EditorConsoleSink final : public spdlog::sinks::base_sink<Mutex>
	{
	public:
		EditorConsoleSink(ConsolePanel* console)
			: m_ConsolePanel(console) {}

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

			m_Messages[m_MessageCount++] = {
				.Level = [&msg]() -> LogLevel  {
					if (msg.level == spdlog::level::trace) return LogLevel::LOG_LEVEL_TRACE;
					else if (msg.level == spdlog::level::debug) return LogLevel::LOG_LEVEL_DEBUG;
					else if (msg.level == spdlog::level::info) return LogLevel::LOG_LEVEL_INFO;
					else if (msg.level == spdlog::level::warn) return LogLevel::LOG_LEVEL_WARN;
					else if (msg.level == spdlog::level::err) return LogLevel::LOG_LEVEL_ERROR;
					else if (msg.level == spdlog::level::critical) return LogLevel::LOG_LEVEL_FATAL;
					
					return LogLevel::LOG_LEVEL_TRACE;
				}(),
				.Message = formatted.data(),
				.Time = std::chrono::system_clock::to_time_t(msg.time)
			};
		}

		void flush_() override
		{
			for (int i = 0; i < m_MessageCount; i++) {
				m_ConsolePanel->AddMessage(m_Messages[i]);
			}

			m_MessageCount = 0;
		}

	private:
		ConsolePanel* m_ConsolePanel = nullptr;

		std::array<ConsoleMessage, MAX_MESSAGES> m_Messages;
		int m_MessageCount = 0;
	};

}
