#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/Drop.h"

namespace L2Bot::Domain::Repositories
{
	class DropRepositoryInterface : public ObjectRepositoryInterface<DTO::Drop>
	{
	public:
		virtual const std::map<uint32_t, DTO::Drop> GetObjects() override = 0;
	};
}
