#pragma once
#include <cstdint>
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../Enums/SpoilStateEnum.h"

namespace L2Bot::Domain::DTO
{
	struct NPC : public WorldObject
	{
	public:
		const bool isHostile = false;
		const uint32_t npcId = 0;
		const Enums::SpoilStateEnum spoilState = Enums::SpoilStateEnum::none;
		const ValueObjects::FullName fullName = ValueObjects::FullName();
		const ValueObjects::VitalStats vitalStats = ValueObjects::VitalStats();
	};
}
