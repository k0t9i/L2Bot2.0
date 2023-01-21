#pragma once
#include <map>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/PlayerFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Services/EntityHandler.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class PlayerRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::vector<DTO::EntityState*> GetEntities() override
		{
			const auto creatures = FindAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::map<uint32_t, User*> items;
			for (const auto& kvp : creatures)
			{
				const auto creature = kvp.second;
				if (creature->userType == L2::UserType::USER && creature->lvl == 0) {
					items.emplace(creature->objectId, creature);
				}
			}

			const auto objects = m_EntityHandler.GetEntities<User*>(items, [this](User* item) {
				return m_Factory.Create(item);
			});

			auto result = std::vector<DTO::EntityState*>();

			for (const auto kvp : objects)
			{
				result.push_back(kvp.second);
			}

			return result;
		}

		void Reset() override
		{
			m_EntityHandler.Reset();
		}

		PlayerRepository(const NetworkHandlerWrapper& networkHandler, const PlayerFactory& factory, EntityHandler& handler, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius),
			m_EntityHandler(handler)
		{

		}

		PlayerRepository() = delete;
		virtual ~PlayerRepository() = default;

	private:
		const PlayerFactory& m_Factory;
		const NetworkHandlerWrapper& m_NetworkHandler;
		const uint16_t m_Radius;
		EntityHandler& m_EntityHandler;
	};
}