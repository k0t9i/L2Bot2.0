#pragma once

#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/DTO/EntityState.h"
#include "../Factories/DropFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../GameStructs/NetworkHandlerWrapper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class DropRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto allItems = FindAllObjects<Item*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextItem(radius, prevId);
			});

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result;
			for (const auto kvp : allItems) {
				const auto item = kvp.second;
				if (m_Drops.find(item->objectId) == m_Drops.end()) {
					m_Drops[item->objectId] = m_Factory.Create(item);
				}
				else
				{
					m_Factory.Update(m_Drops[item->objectId], item);
				}
				result[item->objectId] = m_Drops[item->objectId];
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Drops.clear();
		}

		DropRepository(const NetworkHandlerWrapper& networkHandler, const DropFactory& factory, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius)
		{

		}

		DropRepository() = delete;
		virtual ~DropRepository() = default;

	private:
		const NetworkHandlerWrapper& m_NetworkHandler;
		const DropFactory& m_Factory;
		const uint16_t m_Radius;
		std::shared_timed_mutex m_Mutex;
		std::unordered_map<uint32_t, std::shared_ptr<Entities::Drop>> m_Drops;
	};
}
