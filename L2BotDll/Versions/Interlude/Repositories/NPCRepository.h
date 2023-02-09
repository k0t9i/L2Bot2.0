#pragma once

#include <map>
#include <shared_mutex>
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/NPCFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/SpoiledEvent.h"
#include "../../../Events/CreatureDiedEvent.h"
#include "../../GameStructs/FindObjectsTrait.h"
#include "../../../Services/EntityFinder.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class NPCRepository : public Repositories::EntityRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto creatures = FindAllObjects<User*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextCreature(radius, prevId);
			});

			std::map<uint32_t, User*> items;
			for (const auto& kvp : creatures)
			{
				const auto creature = kvp.second;
				if (creature->userType == L2::UserType::NPC) {
					items.emplace(creature->objectId, creature);
				}
			}

			const auto objects = m_EntityFinder.FindEntities<User*>(items, [this](User* item) {
				const auto spoilState = m_Spoiled.find(item->objectId) == m_Spoiled.end() ? Enums::SpoilStateEnum::none : m_Spoiled.at(item->objectId);
				return m_Factory.Create(item, spoilState);
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

		NPCRepository(const NetworkHandlerWrapper& networkHandler, const NPCFactory& factory, EntityFinder& finder, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius),
			m_EntityFinder(finder)
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
					const auto isSweepable = casted.GetCreatureInfo()[4] != 0;
					if (m_Spoiled[casted.GetCreatureId()] == Enums::SpoilStateEnum::spoiled && isSweepable)
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
		EntityFinder& m_EntityFinder;
		std::shared_timed_mutex m_Mutex;
	};
}