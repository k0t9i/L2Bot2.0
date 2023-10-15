#pragma once
#include <unordered_map>
#include "../Entities/EntityInterface.h"

namespace L2Bot::Domain::Repositories
{
	class EntityRepositoryInterface
	{
	public:
		virtual const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() = 0;
		virtual void Reset() = 0;
	};
}
