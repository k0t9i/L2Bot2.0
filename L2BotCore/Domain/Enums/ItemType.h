#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class ItemType : int8_t
	{
		none = -1,
		etc,
		armor,
		weapon,
		shield
	};
}