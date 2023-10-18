#pragma once

#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/HeroFactory.h"
#include "Domain/Events/HeroCreatedEvent.h"
#include "Domain/Events/HeroDeletedEvent.h"
#include "Domain/Events/CreatureDiedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "Domain/Services/ServiceLocator.h"

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
					Services::ServiceLocator::GetInstance().GetEventDispatcher()->Dispatch(Events::HeroCreatedEvent{});
					Services::ServiceLocator::GetInstance().GetLogger()->App(L"{} enter in the world", m_Hero->GetFullName().GetNickname());
				}
				else
				{
					m_Factory.Update(m_Hero, hero);
				}
				result[hero->objectId] = m_Hero;
			}
			else if (m_Hero) {
				Services::ServiceLocator::GetInstance().GetLogger()->App(L"{} leave the world", m_Hero->GetFullName().GetNickname());
				m_Hero = nullptr;
				Services::ServiceLocator::GetInstance().GetEventDispatcher()->Dispatch(Events::HeroDeletedEvent{});
			}

			return result;
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Hero = nullptr;
		}

		void Init() override
		{
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::CreatureDiedEvent::name, [this](const Events::Event& evt) {
				std::unique_lock<std::shared_timed_mutex>(m_Mutex);
				if (evt.GetName() == Events::CreatureDiedEvent::name)
				{
					const auto casted = static_cast<const Events::CreatureDiedEvent&>(evt);
					if (m_Hero && m_Hero->GetId() == casted.GetCreatureId())
					{
						Services::ServiceLocator::GetInstance().GetLogger()->App(L"{} died", m_Hero->GetFullName().GetNickname());
					}
				}
			});
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