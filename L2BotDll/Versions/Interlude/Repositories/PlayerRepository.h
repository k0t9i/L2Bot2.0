#pragma once
#include <map>
#include "Domain/Repositories/PlayerRepositoryInterface.h"
#include "../Factories/PlayerFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../GameStructs/NetworkHandlerWrapper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class PlayerRepository : public Repositories::PlayerRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::map<uint32_t, DTO::Player> GetObjects() override
		{
			const auto creatures = GetAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::map<uint32_t, DTO::Player> map;

			for (const auto& kvp : creatures)
			{
				const auto creature = kvp.second;
				if (creature->userType == L2::UserType::USER && creature->lvl == 0) {
					map.emplace(creature->objectId, m_Factory.Create(creature));
				}
			}

			return map;
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
	};
}