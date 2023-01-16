#pragma once
#include <cstdint>
#include <string>
#include "WorldObject.h"

namespace L2Bot::Domain::DTO
{
	struct Drop : public WorldObject
	{
	public:
		const uint32_t itemId = 0;
		const uint32_t amount = 0;
		const std::string name = "";
		const std::string iconName = "";
	};
}
