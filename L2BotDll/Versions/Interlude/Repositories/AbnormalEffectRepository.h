#pragma once

#include <unordered_map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/AbnormalEffectFactory.h"
#include "Domain/Events/AbnormalEffectChangedEvent.h"
#include "Domain/Events/HeroDeletedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "Domain/Services/ServiceLocator.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class AbnormalEffectRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			return m_Effects;
		}

		AbnormalEffectRepository(const AbnormalEffectFactory& factory) :
			m_Factory(factory)
		{
		}

		AbnormalEffectRepository() = delete;
		virtual ~AbnormalEffectRepository() = default;

		void Init() override
		{
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::AbnormalEffectChangedEvent::name, [this](const Events::Event& evt) {
				OnEffectToggled(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::HeroDeletedEvent::name, [this](const Events::Event& evt) {
				OnHeroDeleted(evt);
			});
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Effects.clear();
		}

		void OnHeroDeleted(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::HeroDeletedEvent::name)
			{
				Reset();
			}
		}

		void OnEffectToggled(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::AbnormalEffectChangedEvent::name)
			{
				const auto casted = static_cast<const Events::AbnormalEffectChangedEvent&>(evt);
				
				const auto &actualIds = Create(casted.GetSkillInfo());
				Delete(actualIds);
			}
		}

	private:
		const std::unordered_map<uint32_t, int32_t> Create(const std::vector<int32_t> &skillInfo)
		{
			std::unordered_map<uint32_t, int32_t> ids;

			for (size_t i = 0; i < skillInfo.size(); i += 3)
			{
				const auto effectId = skillInfo[i];
				const auto level = skillInfo[i + 1];

				m_Effects[effectId] = m_Factory.Create(effectId, level);

				ids[effectId] = effectId;
			}

			return ids;
		}

		void Delete(const std::unordered_map<uint32_t, int32_t> &actualIds)
		{
			for (auto it = m_Effects.begin(); it != m_Effects.end();)
			{
				if (actualIds.find(it->second->GetId()) == actualIds.end())
				{
					it = m_Effects.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

	private:
		const AbnormalEffectFactory& m_Factory;
		std::unordered_map<uint32_t, std::shared_ptr<Entities::EntityInterface>> m_Effects;
		std::shared_timed_mutex m_Mutex;
	};
}