#pragma once

#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/HeroFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/HeroCreatedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class HeroRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto hero = m_NetworkHandler.GetHero();

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result;
			if (hero) {
				if (!m_Hero) {
					m_Hero = m_Factory.Create(hero);
					EventDispatcher::GetInstance().Dispatch(HeroCreatedEvent{});
				}
				else
				{
					m_Factory.Update(m_Hero, hero);
				}
				result[hero->objectId] = m_Hero;
			}
			else {
				m_Hero = nullptr;
				EventDispatcher::GetInstance().Dispatch(HeroDeletedEvent{});
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Hero = nullptr;
		}

		HeroRepository(const NetworkHandlerWrapper& networkHandler, const HeroFactory& factory) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory)
		{

		}

		HeroRepository() = delete;
		virtual ~HeroRepository() = default;

	private:
		const HeroFactory& m_Factory;
		const NetworkHandlerWrapper& m_NetworkHandler;
		std::shared_ptr<Entities::Hero> m_Hero;
	};
}