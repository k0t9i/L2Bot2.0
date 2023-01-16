#pragma once
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../ValueObjects/Phenotype.h"
#include "../ValueObjects/ExperienceInfo.h"
#include "../ValueObjects/PermanentStats.h"
#include "../ValueObjects/VariableStats.h"
#include "../ValueObjects/Reputation.h"
#include "../ValueObjects/InventoryInfo.h"
#include <cstdint>

namespace L2Bot::Domain::DTO
{
	struct Hero : public WorldObject
	{
	public:
		const ValueObjects::FullName fullName = ValueObjects::FullName();
		const ValueObjects::VitalStats vitalStats = ValueObjects::VitalStats();
		const ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();
		const ValueObjects::ExperienceInfo experienceInfo = ValueObjects::ExperienceInfo();
		const ValueObjects::PermanentStats permanentStats = ValueObjects::PermanentStats();
		const ValueObjects::VariableStats variableStats = ValueObjects::VariableStats();
		const ValueObjects::Reputation reputation = ValueObjects::Reputation();
		const ValueObjects::InventoryInfo inventoryInfo = ValueObjects::InventoryInfo();
		const uint32_t targetId = 0;
		const bool isStanding = true;
	};
}
