#pragma once

#include <Windows.h>
#include "Domain/Logger/LogChannel.h"

using namespace L2Bot::Domain;

class OutputDebugLogChannel : public Logger::LogChannel
{
public:
	OutputDebugLogChannel(const std::vector<Logger::LogLevel> levels) : Logger::LogChannel(levels) {};
	virtual ~OutputDebugLogChannel() = default;

protected:
	void DoSendToChannel(const std::wstring& logEntry) override
	{
		OutputDebugStringW(logEntry.c_str());
	}
};