#pragma once

#include "Domain/Repositories/HeroRepositoryInterface.h"
#include "../Factories/HeroFactory.h"
#include "../../../Events/EventDispatcher.h"
#include "../../../Events/HeroCreatedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class HeroRepository : public Repositories::HeroRepositoryInterface
	{
	public:
		const std::map<uint32_t, DTO::Hero> GetObjects() override
		{
			std::map<uint32_t, DTO::Hero> map;
			const auto hero = m_NetworkHandler.GetHero();
			if (hero)
			{
				map.emplace(hero->objectId, m_Factory.Create(hero));
			}

			if (hero != nullptr && m_PrevHero == nullptr)
			{
				EventDispatcher::GetInstance().Dispatch(HeroCreatedEvent{});
			}
			if (hero == nullptr && m_PrevHero != nullptr)
			{
				EventDispatcher::GetInstance().Dispatch(HeroDeletedEvent{});
			}

			m_PrevHero = hero;

			return map;
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
		User* m_PrevHero = nullptr;
	};
}