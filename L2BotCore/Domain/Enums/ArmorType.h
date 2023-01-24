#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class ArmorType : uint8_t
	{
		none = 0,
		light,
		heavy,
		robe
	};
}