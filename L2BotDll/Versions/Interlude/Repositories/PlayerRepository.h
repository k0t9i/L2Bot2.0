#pragma once
#include <unordered_map>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/PlayerFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../GameStructs/NetworkHandlerWrapper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class PlayerRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			const auto allCreatures = FindAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result;
			for (const auto& kvp : allCreatures)
			{
				const auto &creature = kvp.second;
				if (creature->userType != L2::UserType::USER || creature->lvl != 0) {
					continue;
				}

				if (m_Players.find(creature->objectId) == m_Players.end()) {
					m_Players[creature->objectId] = m_Factory.Create(creature);
				}
				else
				{
					m_Factory.Update(m_Players[creature->objectId], creature);
				}

				result[creature->objectId] = m_Players[creature->objectId];
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Players.clear();
		}

		PlayerRepository(const NetworkHandlerWrapper& networkHandler, const PlayerFactory& factory, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius)
		{

		}

		PlayerRepository() = delete;
		virtual ~PlayerRepository() = default;

	private:
		const PlayerFactory& m_Factory;
		const NetworkHandlerWrapper& m_NetworkHandler;
		const uint16_t m_Radius;
		std::unordered_map<uint32_t, std::shared_ptr<Entities::Player>> m_Players;
	};
}