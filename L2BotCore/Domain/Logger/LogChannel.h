#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "LogLevel.h"

namespace L2Bot::Domain::Logger
{
	class LogChannel
	{
	public:
		LogChannel(const std::vector<LogLevel> levels) : m_Levels(levels) {};
		virtual ~LogChannel() = default;

		void SendToChannel(const std::wstring& logEntry)
		{
			DoSendToChannel(logEntry);
		}

		const bool IsAppropriateLevel(const LogLevel level) const
		{
			return m_Levels.size() == 0 || std::find(m_Levels.begin(), m_Levels.end(), level) != m_Levels.end();
		}

	protected:
		virtual void DoSendToChannel(const std::wstring& logEntry) = 0;

		const std::wstring GetCurrentDateTime() const
		{
			time_t rawTime = std::time(nullptr);
			struct tm timeinfo;
			localtime_s(&timeinfo, &rawTime);
			std::wstringstream oss;
			oss << "[" << std::put_time(&timeinfo, L"%Y-%m-%d %H:%M:%S") << "]";
			return oss.str();
		}

	private:
		const std::vector<LogLevel> m_Levels;
	};
}