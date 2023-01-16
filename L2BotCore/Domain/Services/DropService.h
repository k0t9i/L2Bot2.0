#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/Drop.h"
#include "../Entities/Drop.h"
#include "../Repositories/DropRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class DropService : public ObjectService<Entities::Drop, DTO::Drop>
	{
	public:
		DropService(Repositories::DropRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~DropService() override = default;
	};
}
