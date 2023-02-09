#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include <math.h>
#include "../DTO/EntityState.h"
#include "../Entities/WorldObject.h"
#include "../Repositories/EntityRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class EntityHandler
	{
	public:
		EntityHandler(Repositories::EntityRepositoryInterface& repository) : m_Repository(repository)
		{

		}
		EntityHandler() = delete;
		virtual ~EntityHandler() = default;

		virtual const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities()
		{
			return m_Repository.GetEntities();
		}

		void Invalidate()
		{
			m_Repository.Reset();
		}
	private:
		Repositories::EntityRepositoryInterface& m_Repository;
	};
}
