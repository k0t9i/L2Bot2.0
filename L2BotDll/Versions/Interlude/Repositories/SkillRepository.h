#pragma once

#include <map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/SkillRepositoryInterface.h"
#include "../Factories/SkillFactory.h"
#include "../../../Events/SkillCreatedEvent.h"
#include "../../../Events/SkillUsedEvent.h"
#include "../../../Events/SkillCancelledEvent.h"
#include "../../../Events/AbnormalEffectChangedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Common/TimerMap.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class SkillRepository : public Repositories::SkillRepositoryInterface
	{
	public:
		const std::map<uint32_t, DTO::Skill> GetObjects() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);
			return m_Skills;
		}

		SkillRepository(const NetworkHandlerWrapper& networkHandler, const SkillFactory& factory) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory)
		{
			EventDispatcher::GetInstance().Subscribe(SkillCreatedEvent::name, [this](const Event& evt) {
				OnSkillCreated(evt);
			});
			EventDispatcher::GetInstance().Subscribe(SkillUsedEvent::name, [this](const Event& evt) {
				OnSkillUsed(evt);
			});
			EventDispatcher::GetInstance().Subscribe(SkillCancelledEvent::name, [this](const Event& evt) {
				OnSkillCancelled(evt);
			});
			EventDispatcher::GetInstance().Subscribe(AbnormalEffectChangedEvent::name, [this](const Event& evt) {
				OnSkillToggled(evt);
			});
			EventDispatcher::GetInstance().Subscribe(HeroDeletedEvent::name, [this](const Event& evt) {
				OnHeroDeleted(evt);
			});
		}

		SkillRepository() = delete;
		virtual ~SkillRepository() = default;

		void OnHeroDeleted(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == HeroDeletedEvent::name)
			{
				m_Skills.clear();
				m_CastingTimers.StopAll();
				m_ReloadingTimers.StopAll();
			}
		}
		void OnSkillCreated(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == SkillCreatedEvent::name)
			{
				const auto casted = static_cast<const SkillCreatedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();
				const auto skillId = skillInfo[2];

				const auto alreadyExists = m_Skills.find(skillId) != m_Skills.end();

				auto skill = m_Factory.Create(
					alreadyExists ? m_Skills[skillId] : DTO::Skill(),
					skillInfo[2],
					skillInfo[1],
					skillInfo[0]
				);

				UpdateSkill(skill);
			}
		}
		void OnSkillUsed(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == SkillUsedEvent::name)
			{
				const auto casted = static_cast<const SkillUsedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();
				const auto skillId = skillInfo[0];

				if (m_Skills.find(skillId) == m_Skills.end())
				{
					//todo exception?
					return;
				}

				auto skill = m_Factory.UpdateReloadingState(
					m_Factory.UpdateCastingState(
						m_Skills[skillId],
						true
					),
					true
				);

				UpdateSkill(skill);
				m_UsedSkillId = skill.skillId;

				m_ReloadingTimers.StartTimer(skill.skillId, skillInfo[3], [this] (uint32_t skillId) {
					std::shared_lock<std::shared_timed_mutex>(m_Mutex);
					auto skill = m_Factory.UpdateReloadingState(
						m_Skills[skillId],
						false
					);
					UpdateSkill(skill);
				});
				m_CastingTimers.StartTimer(skill.skillId, skillInfo[2], [this] (uint32_t skillId) {
					std::shared_lock<std::shared_timed_mutex>(m_Mutex);
					auto skill = m_Factory.UpdateCastingState(
						m_Skills[m_UsedSkillId],
						false
					);
					UpdateSkill(skill);
				});
			}
		}
		void OnSkillCancelled(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == SkillCancelledEvent::name)
			{
				const auto casted = static_cast<const SkillCancelledEvent&>(evt);

				const auto hero = m_NetworkHandler.GetHero();

				if (hero && hero->objectId == casted.GetInitiatorId())
				{
					if (m_Skills.find(m_UsedSkillId) == m_Skills.end())
					{
						//todo exception?
						return;
					}

					auto skill = m_Factory.UpdateCastingState(
						m_Skills[m_UsedSkillId],
						false
					);

					UpdateSkill(skill);
					m_UsedSkillId = 0;

					m_CastingTimers.StopTimer(skill.skillId);
				}
			}
		}
		void OnSkillToggled(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == AbnormalEffectChangedEvent::name)
			{
				const auto casted = static_cast<const AbnormalEffectChangedEvent&>(evt);
				const auto skillInfo = casted.GetSkillInfo();

				std::map<uint32_t, int32_t> ids;

				for (size_t i = 0; i < skillInfo.size(); i += 3)
				{
					ids[skillInfo[i]] = skillInfo[i + 2];
				}

				for (auto it = m_Skills.begin(); it != m_Skills.end();)
				{
					const auto needToToggle = ids.find(it->second.skillId) != ids.end();
					// buff time less than zero means this is a aura
					const auto isAura = needToToggle ? ids[it->second.skillId] < 0 : false;

					if (it->second.isToggled && !needToToggle)
					{
						auto skill = m_Factory.UpdateToggle(it->second, false);
						it = m_Skills.erase(it);
						m_Skills.emplace(skill.skillId, skill);
					}
					else if (!it->second.isToggled && needToToggle && isAura)
					{
						auto skill = m_Factory.UpdateToggle(it->second, true);
						it = m_Skills.erase(it);
						m_Skills.emplace(skill.skillId, skill);
					}
					else
					{
						++it;
					}
				}
			}
		}

	private:
		void UpdateSkill(const DTO::Skill skill)
		{
			m_Skills.erase(skill.skillId);
			m_Skills.emplace(skill.skillId, skill);
		}

	private:
		const SkillFactory& m_Factory;
		std::map<uint32_t, DTO::Skill> m_Skills;
		uint32_t m_UsedSkillId = 0;
		const NetworkHandlerWrapper& m_NetworkHandler;
		TimerMap m_ReloadingTimers;
		TimerMap m_CastingTimers;
		std::shared_timed_mutex m_Mutex;
	};
}