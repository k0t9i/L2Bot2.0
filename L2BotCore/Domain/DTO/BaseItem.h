#pragma once
#include <cstdint>
#include <string>
#include "WorldObject.h"

namespace L2Bot::Domain::DTO
{
	struct BaseItem
	{
	public:
		const uint32_t itemId = 0;
		const uint32_t amount = 0;
		const bool isEquipped = 0;
		const uint16_t enchantLevel = 0;
		const int32_t mana = -1;
		const std::string name = "";
		const std::string iconName = "";
		const std::string description = "";
		const uint16_t weight = 0;
	};
}
