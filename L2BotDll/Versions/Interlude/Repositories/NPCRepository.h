#pragma once

#include <map>
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "Domain/Repositories/NPCRepositoryInterface.h"
#include "../Factories/NPCFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/SpoiledEvent.h"
#include "../../../Events/CreatureDiedEvent.h"
#include "../../GameStructs/FindObjectsTrait.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class NPCRepository : public Repositories::NPCRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::map<uint32_t, DTO::NPC> GetObjects() override
		{
			const auto creatures = GetAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::map<uint32_t, DTO::NPC> map;

			for (const auto& kvp : creatures)
			{
				const auto creature = kvp.second;
				if (creature->userType == L2::UserType::NPC) {
					const auto spoilState = m_Spoiled.find(creature->objectId) == m_Spoiled.end() ? Enums::SpoilStateEnum::none : m_Spoiled.at(creature->objectId);
					map.emplace(creature->objectId, m_Factory.Create(creature, spoilState));
				}
			}

			return map;
		}

		NPCRepository(const NetworkHandlerWrapper& networkHandler, const NPCFactory& factory, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius)
		{
			EventDispatcher::GetInstance().Subscribe(SpoiledEvent::name, [this](const Event& evt) {
				OnSpoiled(evt);
			});
			EventDispatcher::GetInstance().Subscribe(CreatureDiedEvent::name, [this](const Event& evt) {
				OnCreatureDied(evt);
			});
		}

		NPCRepository() = delete;
		virtual ~NPCRepository() = default;

		void OnSpoiled(const Event& evt)
		{
			if (evt.GetName() == SpoiledEvent::name)
			{
				const auto casted = static_cast<const SpoiledEvent&>(evt);
				const auto hero = m_NetworkHandler.GetHero();
				if (hero && hero->pawn && hero->pawn->lineagePlayerController)
				{
					const auto targetId = hero->pawn->lineagePlayerController->targetObjectId;
					if (targetId)
					{
						m_Spoiled[targetId] = Enums::SpoilStateEnum::spoiled;
					}
				}
			}
		}

		void OnCreatureDied(const Event& evt)
		{
			if (evt.GetName() == CreatureDiedEvent::name)
			{
				const auto casted = static_cast<const CreatureDiedEvent&>(evt);
				if (m_Spoiled.find(casted.GetCreatureId()) != m_Spoiled.end())
				{
					if (m_Spoiled[casted.GetCreatureId()] == Enums::SpoilStateEnum::spoiled)
					{
						m_Spoiled[casted.GetCreatureId()] = Enums::SpoilStateEnum::sweepable;
					}
					else
					{
						m_Spoiled[casted.GetCreatureId()] = Enums::SpoilStateEnum::none;
					}
				}
			}
		}

	private:
		const NPCFactory& m_Factory;
		std::map<uint32_t, Enums::SpoilStateEnum> m_Spoiled;
		const NetworkHandlerWrapper& m_NetworkHandler;
		const uint16_t m_Radius = 0;
	};
}