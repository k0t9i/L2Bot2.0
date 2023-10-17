#pragma once

#include <Windows.h>
#include <fstream>
#include "Domain/Logger/LogChannel.h"

using namespace L2Bot::Domain;

class FileLogChannel : public Logger::LogChannel
{
public:
	FileLogChannel(const std::wstring& path, const std::vector<Logger::LogLevel> levels) :
		m_FileStream(path.c_str(), std::wofstream::app),
		Logger::LogChannel(levels)
	{
	};
	virtual ~FileLogChannel()
	{
		m_FileStream.close();
	}

protected:
	void DoSendToChannel(const std::wstring& logEntry) override
	{
		m_FileStream << GetCurrentDateTime() << logEntry << std::endl;
	}

private:
	std::wofstream m_FileStream;
};