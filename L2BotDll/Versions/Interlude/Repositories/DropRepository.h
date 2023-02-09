#pragma once

#include <map>
#include <memory>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/DTO/EntityState.h"
#include "../Factories/DropFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Services/EntityFinder.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class DropRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const std::map<uint32_t, Item*> items = FindAllObjects<Item*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextItem(radius, prevId);
			});
			const auto objects = m_EntityFinder.FindEntities<Item*>(items, [this](Item* item) {
				return m_Factory.Create(item);
			});

			auto result = std::vector<std::shared_ptr<DTO::EntityState>>();

			for (const auto kvp : objects)
			{
				result.push_back(kvp.second);
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_EntityFinder.Reset();
		}

		DropRepository(const NetworkHandlerWrapper& networkHandler, const DropFactory& factory, EntityFinder& finder, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius),
			m_EntityFinder(finder)
		{

		}

		DropRepository() = delete;
		virtual ~DropRepository() = default;

	private:
		const NetworkHandlerWrapper& m_NetworkHandler;
		const DropFactory& m_Factory;
		const uint16_t m_Radius;
		EntityFinder& m_EntityFinder;
		std::shared_timed_mutex m_Mutex;
	};
}
