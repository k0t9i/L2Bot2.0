#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class ItemTypeEnum : int8_t
	{
		none = -1,
		etc,
		armor,
		weapon,
		shield
	};
}