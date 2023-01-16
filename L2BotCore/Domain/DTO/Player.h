#pragma once
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../ValueObjects/Phenotype.h"
namespace L2Bot::Domain::DTO
{
	struct Player : public WorldObject
	{
	public:
		const ValueObjects::FullName fullName = ValueObjects::FullName();
		const ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();
	};
}
