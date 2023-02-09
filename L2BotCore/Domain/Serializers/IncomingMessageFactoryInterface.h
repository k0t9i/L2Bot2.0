#pragma once
#include <string>
#include <vector>
#include "IncomingMessage.h"

namespace L2Bot::Domain::Serializers
{
	class IncomingMessageFactoryInterface
	{
	public:
		virtual const IncomingMessage CreateMessage(std::wstring data) const = 0;
	};
}
