#pragma once
#include <string>

namespace L2Bot::Domain::Transports
{
	class TransportInterface
	{
	public:
		virtual const bool Connect() = 0;
		virtual const bool IsConnected() const = 0;
		virtual const void Send(const std::wstring& data) = 0;
		virtual const std::wstring Receive() = 0;
	};
}
