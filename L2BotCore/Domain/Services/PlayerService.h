#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/Player.h"
#include "../Entities/Player.h"
#include "../Repositories/PlayerRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class PlayerService : public ObjectService<Entities::Player, DTO::Player>
	{
	public:
		PlayerService(Repositories::PlayerRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~PlayerService() override = default;
	};
}
