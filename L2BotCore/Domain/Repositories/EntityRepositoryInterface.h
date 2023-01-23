#pragma once
#include <vector>
#include <memory>
#include "../Entities/WorldObject.h"
#include "../DTO/EntityState.h"

namespace L2Bot::Domain::Repositories
{
	class EntityRepositoryInterface
	{
	public:
		virtual const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() = 0;
		virtual void Reset() = 0;
	};
}
