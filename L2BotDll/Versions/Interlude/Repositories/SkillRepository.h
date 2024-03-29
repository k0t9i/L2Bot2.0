#pragma once

#include <unordered_map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/SkillFactory.h"
#include "Domain/Events/SkillCreatedEvent.h"
#include "Domain/Events/SkillUsedEvent.h"
#include "Domain/Events/SkillCancelledEvent.h"
#include "Domain/Events/AbnormalEffectChangedEvent.h"
#include "Domain/Events/HeroDeletedEvent.h"
#include "Domain/Events/GameEngineTickedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Common/TimerMap.h"
#include "Domain/Services/ServiceLocator.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class SkillRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result(m_Skills.begin(), m_Skills.end());
			return result;
		}

		SkillRepository(const NetworkHandlerWrapper& networkHandler, const SkillFactory& factory) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory)
		{
			
		}

		SkillRepository() = delete;
		virtual ~SkillRepository() = default;

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_CastingTimers.StopAll();
			m_ReloadingTimers.StopAll();
			m_Skills.clear();
			m_IsNewCycle = false;
			m_NewSkills.clear();
		}

		void Init() override
		{
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::SkillCreatedEvent::name, [this](const Events::Event& evt) {
				OnSkillCreated(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::SkillUsedEvent::name, [this](const Events::Event& evt) {
				OnSkillUsed(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::SkillCancelledEvent::name, [this](const Events::Event& evt) {
				OnSkillCancelled(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::AbnormalEffectChangedEvent::name, [this](const Events::Event& evt) {
				OnSkillToggled(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::HeroDeletedEvent::name, [this](const Events::Event& evt) {
				OnHeroDeleted(evt);
			});
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::GameEngineTickedEvent::name, [this](const Events::Event& evt) {
				OnGameEngineTicked(evt);
			});
		}

		void OnGameEngineTicked(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::GameEngineTickedEvent::name)
			{
				for (auto it = m_Skills.begin(); it != m_Skills.end();)
				{
					if (m_NewSkills.find(it->first) == m_NewSkills.end())
					{
						it = m_Skills.erase(it);
					}
					else
					{
						++it;
					}
				}

				m_IsNewCycle = true;
			}
		}

		void OnHeroDeleted(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::HeroDeletedEvent::name)
			{
				Reset();
			}
		}

		//todo need to delete skills if they are not exists in create "queue"
		void OnSkillCreated(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::SkillCreatedEvent::name)
			{
				if (m_IsNewCycle)
				{
					m_IsNewCycle = false;
					m_NewSkills.clear();
				}

				const auto casted = static_cast<const Events::SkillCreatedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();
				const auto skillId = skillInfo[2];

				if (m_Skills.find(skillId) == m_Skills.end())
				{
					const auto newSkill = m_Factory.Create(
						skillInfo[2],
						skillInfo[1],
						skillInfo[0]
					);
					if (newSkill) {
						m_Skills[skillId] = newSkill;
					}
				}
				else
				{
					auto skill = m_Skills[skillId];
					m_Factory.Update(
						skill,
						skillInfo[2],
						skillInfo[1],
						skillInfo[0]
					);
				}
				m_NewSkills[skillId] = skillId;
			}
		}
		void OnSkillUsed(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::SkillUsedEvent::name)
			{
				const auto casted = static_cast<const Events::SkillUsedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();
				const auto skillId = skillInfo[0];

				if (m_Skills.find(skillId) == m_Skills.end())
				{
					//todo exception?
					return;
				}

				const auto& skill = m_Skills[skillId];

				skill->StartReloading();
				skill->StartCasting();

				m_UsedSkillId = skill->GetId();

				m_ReloadingTimers.StartTimer(skill->GetId(), skillInfo[3], [this] (uint32_t skillId) {
					std::shared_lock<std::shared_timed_mutex>(m_Mutex);
					m_Skills[skillId]->StopReloading();
				});
				m_CastingTimers.StartTimer(skill->GetId(), skillInfo[2], [this] (uint32_t skillId) {
					std::shared_lock<std::shared_timed_mutex>(m_Mutex);
					m_Skills[skillId]->StopCasting();
				});
			}
		}
		void OnSkillCancelled(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::SkillCancelledEvent::name)
			{
				const auto casted = static_cast<const Events::SkillCancelledEvent&>(evt);

				const auto hero = m_NetworkHandler.GetHero();

				if (hero && hero->objectId == casted.GetInitiatorId())
				{
					if (m_Skills.find(m_UsedSkillId) == m_Skills.end())
					{
						//todo exception?
						return;
					}

					m_Skills[m_UsedSkillId]->StopCasting();
					m_CastingTimers.StopTimer(m_UsedSkillId);

					m_UsedSkillId = 0;
				}
			}
		}
		void OnSkillToggled(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::AbnormalEffectChangedEvent::name)
			{
				const auto casted = static_cast<const Events::AbnormalEffectChangedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();

				std::unordered_map<uint32_t, int32_t> ids;

				for (size_t i = 0; i < skillInfo.size(); i += 3)
				{
					ids[skillInfo[i]] = skillInfo[i + 2];
				}

				for (const auto& kvp : m_Skills)
				{
					const auto& skill = kvp.second;

					const auto needToToggle = ids.find(skill->GetId()) != ids.end();
					// buff time less than zero means this is a aura
					const auto isAura = needToToggle ? ids[skill->GetId()] < 0 : false;

					if (skill->IsToggled() && !needToToggle)
					{
						skill->ToggleOff();
					}
					else if (!skill->IsToggled() && needToToggle && isAura)
					{
						skill->ToggleOn();
					}
				}
			}
		}

	private:
		const SkillFactory& m_Factory;
		std::unordered_map<std::uint32_t, std::shared_ptr<Entities::Skill>> m_Skills;
		std::unordered_map<std::uint32_t, std::uint32_t> m_NewSkills;
		bool m_IsNewCycle = true;
		uint32_t m_UsedSkillId = 0;
		const NetworkHandlerWrapper& m_NetworkHandler;
		TimerMap m_ReloadingTimers;
		TimerMap m_CastingTimers;
		std::shared_timed_mutex m_Mutex;
	};
}