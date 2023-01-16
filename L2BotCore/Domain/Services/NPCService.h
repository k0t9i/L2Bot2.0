#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/NPC.h"
#include "../Entities/NPC.h"
#include "../Repositories/NPCRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class NPCService : public ObjectService<Entities::NPC, DTO::NPC>
	{
	public:
		NPCService(Repositories::NPCRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~NPCService() override = default;
	};
}
