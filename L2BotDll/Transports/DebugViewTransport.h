#pragma once
#include "Domain/Transports/TransportInterface.h"
#include <Windows.h>
#include <thread>

using namespace L2Bot::Domain;

class DebugViewTransport : public Transports::TransportInterface
{
public:
	const bool Connect() override
	{
		return true;
	}

	const bool IsConnected() const
	{
		return true;
	}

	const void Send(const std::wstring& data) override
	{
		OutputDebugStringW(data.c_str());
	}
	const std::wstring Receive() override
	{
		// delay imitation
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return L"";
	}

	DebugViewTransport() = default;
	virtual ~DebugViewTransport() = default;
};