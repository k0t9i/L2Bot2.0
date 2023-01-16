#pragma once
#include <cstdint>
#include <string>

namespace L2Bot::Domain::DTO
{
	struct Skill
	{
	public:
		const uint32_t skillId = 0;
		const uint8_t level = 0;
		const bool isActive = false;
		const uint8_t cost = 0;
		const int16_t range = 0;
		const std::string name = "";
		const std::string description = "";
		const std::string iconName = "";
		const bool isToggled = false;
		const bool isCasting = false;
		const bool isReloading = false;
	};
}
