#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "EntityInterface.h"

namespace L2Bot::Domain::Entities
{
	class Skill : public EntityInterface
	{
	public:
		const uint32_t GetId() const override
		{
			return m_SkillId;
		}
		const bool IsReadyToUse() const
		{
			return !m_IsCasting && !m_IsReloading;
		}
		const bool IsToggled() const
		{
			return m_IsToggled;
		}

		void UpdateReloadingState(const bool isReloading)
		{
			m_IsReloading = isReloading;
		}
		void UpdateCastingState(const bool isCasting)
		{
			m_IsCasting = isCasting;
		}
		void UpdateToggle(const bool isToggled)
		{
			m_IsToggled = isToggled;
		}
		void UpdateLevel(const uint8_t level)
		{
			m_Level = level;
		}

		void Update(const EntityInterface* other) override
		{
			const Skill* casted = static_cast<const Skill*>(other);
			SaveState();

			m_SkillId = casted->m_SkillId;
			m_Level = casted->m_Level;
			m_IsActive = casted->m_IsActive;
			m_Cost = casted->m_Cost;
			m_Range = casted->m_Range;
			m_Name = casted->m_Name;
			m_Description = casted->m_Description;
			m_IconName = casted->m_IconName;
			m_IsToggled = casted->m_IsToggled;
			m_IsCasting = casted->m_IsCasting;
			m_IsReloading = casted->m_IsReloading;
		}
		void SaveState() override
		{
			m_PrevState =
			{
				m_Cost,
				m_Range,
				m_Description,
				m_IsToggled,
				m_IsCasting,
				m_IsReloading,
				IsReadyToUse(),
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const Skill* casted = static_cast<const Skill*>(other);
			return m_SkillId == casted->m_SkillId &&
				m_Level == casted->m_Level &&
				m_IsActive == casted->m_IsActive &&
				m_Cost == casted->m_Cost &&
				m_Range == casted->m_Range &&
				m_Name == casted->m_Name &&
				m_Description == casted->m_Description &&
				m_IconName == casted->m_IconName &&
				m_IsToggled == casted->m_IsToggled &&
				m_IsCasting == casted->m_IsCasting &&
				m_IsReloading == casted->m_IsReloading;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(m_SkillId) });
			result.push_back({ L"level", std::to_wstring(m_Level) });

			if (m_PrevState.isNewState)
			{
				result.push_back({ L"isActive", std::to_wstring(m_IsActive) });
				result.push_back({ L"name", m_Name });
				result.push_back({ L"iconName", m_IconName });
			}

			if (m_PrevState.isNewState || m_Description != m_PrevState.description)
			{
				result.push_back({ L"description", m_Description });
			}
			if (m_PrevState.isNewState || m_Cost != m_PrevState.cost)
			{
				result.push_back({ L"cost", std::to_wstring(m_Cost) });
			}
			if (m_PrevState.isNewState || m_Range != m_PrevState.range)
			{
				result.push_back({ L"range", std::to_wstring(m_Range) });
			}
			if (m_PrevState.isNewState || m_IsToggled != m_PrevState.isToggled)
			{
				result.push_back({ L"isToggled", std::to_wstring(m_IsToggled) });
			}
			if (m_PrevState.isNewState || m_IsCasting != m_PrevState.isCasting)
			{
				result.push_back({ L"isCasting", std::to_wstring(m_IsCasting) });
			}
			if (m_PrevState.isNewState || m_IsReloading != m_PrevState.isReloading)
			{
				result.push_back({ L"isReloading", std::to_wstring(m_IsReloading) });
			}
			if (m_PrevState.isNewState || IsReadyToUse() != m_PrevState.isReadyToUse)
			{
				result.push_back({ L"isReadyToUse", std::to_wstring(IsReadyToUse()) });
			}

			return result;
		}

		Skill(
			const uint32_t skillId,
			const uint8_t level,
			const bool isActive,
			const uint8_t cost,
			const int16_t range,
			const std::wstring& name,
			const std::wstring& description,
			const std::wstring& iconName,
			const bool isToggled,
			const bool isCasting,
			const bool isReloading
		) :
			m_SkillId(skillId),
			m_Level(level),
			m_IsActive(isActive),
			m_Cost(cost),
			m_Range(range),
			m_Name(name),
			m_Description(description),
			m_IconName(iconName),
			m_IsToggled(isToggled),
			m_IsCasting(isCasting),
			m_IsReloading(isReloading)
		{
			
		}

		Skill(const Skill* other) :
			m_SkillId(other->m_SkillId),
			m_Level(other->m_Level),
			m_IsActive(other->m_IsActive),
			m_Cost(other->m_Cost),
			m_Range(other->m_Range),
			m_Name(other->m_Name),
			m_Description(other->m_Description),
			m_IconName(other->m_IconName),
			m_IsToggled(other->m_IsToggled),
			m_IsCasting(other->m_IsCasting),
			m_IsReloading(other->m_IsReloading)
		{
		}

		Skill() = default;
		virtual ~Skill() = default;

	private:
		struct GetState
		{
			uint8_t cost = 0;
			int16_t range = 0;
			std::wstring description = L"";
			bool isToggled = false;
			bool isCasting = false;
			bool isReloading = false;
			bool isReadyToUse = true;

			bool isNewState = true;
		};

	private:
		uint32_t m_SkillId = 0;
		uint8_t m_Level = 0;
		bool m_IsActive = false;
		uint8_t m_Cost = 0;
		int16_t m_Range = 0;
		std::wstring m_Name = L"";
		std::wstring m_Description = L"";
		std::wstring m_IconName = L"";
		bool m_IsToggled = false;
		bool m_IsCasting = false;
		bool m_IsReloading = false;
		GetState m_PrevState = GetState();
	};
}
