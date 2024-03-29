#pragma once

#include <vector>
#include <format>
#include <memory>
#include "LogChannel.h"
#include "LogLevel.h"

namespace L2Bot::Domain::Logger
{
	class Logger
	{
	public:
		Logger(std::vector<std::unique_ptr<LogChannel>> channels) : m_Channels(std::move(channels)) {};
		virtual ~Logger() = default;

		template <class ... Args>
		void Error(const std::wformat_string<Args...> format, Args... args) const
		{
			Log(LogLevel::error, format, args...);
		}
		template <class ... Args>
		void Warning(const std::wformat_string<Args...> format, Args... args) const
		{
			Log(LogLevel::warning, format, args...);
		}
		template <class ... Args>
		void Info(const std::wformat_string<Args...> format, Args... args) const
		{
			Log(LogLevel::info, format, args...);
		}
		template <class ... Args>
		void App(const std::wformat_string<Args...> format, Args... args) const
		{
			Log(LogLevel::app, format, args...);
		}

		void Error(const std::wstring& message) const
		{
			Log(LogLevel::error, message);
		}
		void Warning(const std::wstring& message) const
		{
			Log(LogLevel::warning, message);
		}
		void Info(const std::wstring& message) const
		{
			Log(LogLevel::info, message);
		}
		void App(const std::wstring& message) const
		{
			Log(LogLevel::app, message);
		}

	private:
		template <class ... Args>
		void Log(LogLevel level, const std::wformat_string<Args...> format, Args... args) const
		{
			Log(level, std::vformat(format.get(), std::make_wformat_args(args...)));
		}
		void Log(LogLevel level, const std::wstring& message) const
		{
			std::wstring prefix = L"";
			if (level == LogLevel::error) {
				prefix = L"[Error]: ";
			}
			else if (level == LogLevel::warning) {
				prefix = L"[Warning]: ";
			}
			else if (level == LogLevel::info) {
				prefix = L"[Info]: ";
			}
			else if (level == LogLevel::app) {
				prefix = L"[App]: ";
			}

			for (const auto& channel : m_Channels) {
				if (channel->IsAppropriateLevel(level)) {
					channel->SendToChannel(prefix + message);
				}
			}
		}

	private:
		const std::vector<std::unique_ptr<LogChannel>> m_Channels;
	};
}
