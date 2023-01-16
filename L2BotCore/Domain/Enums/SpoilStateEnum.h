#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class SpoilStateEnum : uint32_t
	{
		none = 0,
		spoiled,
		sweepable
	};
};