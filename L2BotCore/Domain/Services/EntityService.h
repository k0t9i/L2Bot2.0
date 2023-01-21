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
	class EntityService
	{
	public:
		EntityService(Repositories::EntityRepositoryInterface& repository) : m_Repository(repository)
		{

		}
		EntityService() = delete;
		virtual ~EntityService() = default;

		virtual const std::vector<DTO::EntityState*> GetEntities()
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
