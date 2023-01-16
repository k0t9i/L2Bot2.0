#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/Player.h"

namespace L2Bot::Domain::Repositories
{
	class PlayerRepositoryInterface : public ObjectRepositoryInterface<DTO::Player>
	{
	public:
		virtual const std::map<uint32_t, DTO::Player> GetObjects() override = 0;
	};
}
