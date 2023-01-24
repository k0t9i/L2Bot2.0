#pragma once

#include <cstdint>

struct ItemData
{
	const uint32_t objectId = 0;
	const uint32_t itemId = 0;
	const uint32_t amount = 0;
	const uint16_t isEquipped = 0;
	const uint16_t enchantLevel = 0;
	const int32_t mana = -1;
	const bool isQuest = false;
};