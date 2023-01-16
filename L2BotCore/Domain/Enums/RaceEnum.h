#pragma once
#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class RaceEnum : uint8_t
	{
		none = 255,
		darkElf = 2,
		dwarf = 4,
		elf = 1,
		human = 0,
		orc = 3
	};
}