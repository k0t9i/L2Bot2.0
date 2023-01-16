#pragma once
#include <string>

namespace L2Bot::Domain::Transports
{
	class TransportInterface
	{
	public:
		virtual const bool Connect() = 0;
		virtual const bool IsConnected() const = 0;
		virtual const void Send(std::string data) = 0;
		virtual const std::string Receive() = 0;
	};
}
