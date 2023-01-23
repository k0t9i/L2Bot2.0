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

	const void Send(const std::string& data) override
	{
		OutputDebugStringA(data.c_str());
	}
	const std::string Receive() override
	{
		// delay imitation
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		return "";
	}

	DebugViewTransport() = default;
	virtual ~DebugViewTransport() = default;
};