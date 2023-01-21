#pragma once
#include <vector>
#include "../Entities/WorldObject.h"
#include "../DTO/EntityState.h"

namespace L2Bot::Domain::Repositories
{
	class EntityRepositoryInterface
	{
	public:
		virtual const std::vector<DTO::EntityState*> GetEntities() = 0;
		virtual void Reset() = 0;
	};
}
