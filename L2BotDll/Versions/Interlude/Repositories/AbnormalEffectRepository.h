#pragma once

#include <map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/AbnormalEffectFactory.h"
#include "../../../Events/AbnormalEffectChangedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../../../Events/EventDispatcher.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Services/EntityFinder.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class AbnormalEffectRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto objects = m_EntityFinder.FindEntities<std::shared_ptr<Entities::AbnormalEffect>>(m_Effects, [this](std::shared_ptr<Entities::AbnormalEffect> item) {
				return std::make_shared<Entities::AbnormalEffect>(item.get());
			});

			auto result = std::vector<std::shared_ptr<DTO::EntityState>>();

			for (const auto kvp : objects)
			{
				result.push_back(kvp.second);
			}

			return result;
		}

		AbnormalEffectRepository(const AbnormalEffectFactory& factory, EntityFinder& finder) :
			m_Factory(factory),
			m_EntityFinder(finder)
		{
			EventDispatcher::GetInstance().Subscribe(AbnormalEffectChangedEvent::name, [this](const Event& evt) {
				OnEffectToggled(evt);
			});
			EventDispatcher::GetInstance().Subscribe(HeroDeletedEvent::name, [this](const Event& evt) {
				OnHeroDeleted(evt);
			});
		}

		AbnormalEffectRepository() = delete;
		virtual ~AbnormalEffectRepository()
		{
			Reset();
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Effects.clear();
		}

		void OnHeroDeleted(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == HeroDeletedEvent::name)
			{
				Reset();
			}
		}

		void OnEffectToggled(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == AbnormalEffectChangedEvent::name)
			{
				const auto casted = static_cast<const AbnormalEffectChangedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();

				std::map<uint32_t, int32_t> ids;
				for (size_t i = 0; i < skillInfo.size(); i += 3)
				{
					const auto effectId = skillInfo[i];
					const auto level = skillInfo[i + 1];

					auto effect = m_Factory.Create(effectId, level);
					m_Effects.emplace(effect->GetId(), effect);

					ids[effectId] = effectId;
				}

				for (auto it = m_Effects.begin(); it != m_Effects.end();)
				{
					const auto& effect = it->second;

					if (ids.find(effect->GetId()) == ids.end())
					{
						it = m_Effects.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}

	private:
		const AbnormalEffectFactory& m_Factory;
		std::map<uint32_t, std::shared_ptr<Entities::AbnormalEffect>> m_Effects;
		std::shared_timed_mutex m_Mutex;
		EntityFinder& m_EntityFinder;
	};
}