#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/NPC.h"

namespace L2Bot::Domain::Repositories
{
	class NPCRepositoryInterface : public ObjectRepositoryInterface<DTO::NPC>
	{
	public:
		virtual const std::map<uint32_t, DTO::NPC> GetObjects() override = 0;
	};
}
