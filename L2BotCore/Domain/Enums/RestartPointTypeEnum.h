#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class RestartPointTypeEnum : uint8_t
	{
		village = 0,
		clanhall,
		castle,
		siege,
		festival,
		jail = 27
	};
}