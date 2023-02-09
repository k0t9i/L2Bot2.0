#pragma once

#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/HeroFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/HeroCreatedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Services/EntityFinder.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class HeroRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			auto hero = m_NetworkHandler.GetHero();

			if (hero != nullptr && m_PrevHero == nullptr)
			{
				EventDispatcher::GetInstance().Dispatch(HeroCreatedEvent{});
			}
			if (hero == nullptr && m_PrevHero != nullptr)
			{
				EventDispatcher::GetInstance().Dispatch(HeroDeletedEvent{});
			}
			m_PrevHero = hero;

			std::map<uint32_t, User*> items;
			
			if (hero)
			{
				items.emplace(hero->objectId, hero);
			}

			const auto objects = m_EntityFinder.FindEntities<User*>(items, [this](User* item) {
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

		HeroRepository(const NetworkHandlerWrapper& networkHandler, const HeroFactory& factory, EntityFinder& finder) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_EntityFinder(finder)
		{

		}

		HeroRepository() = delete;
		virtual ~HeroRepository() = default;

	private:
		const HeroFactory& m_Factory;
		const NetworkHandlerWrapper& m_NetworkHandler;
		User* m_PrevHero = nullptr;
		EntityFinder& m_EntityFinder;
		std::shared_timed_mutex m_Mutex;
	};
}