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
		struct Data
		{
			uint32_t skillId;
			uint8_t level;
			bool isActive;
			uint8_t cost;
			int16_t range;
			std::wstring name;
			std::wstring description;
			std::wstring iconName;
		};

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

		void StartReloading()
		{
			m_IsReloading = true;
		}
		void StopReloading()
		{
			m_IsReloading = false;
		}
		void StartCasting()
		{
			m_IsCasting = true;
		}
		void StopCasting()
		{
			m_IsCasting = false;
		}
		void ToggleOn()
		{
			m_IsToggled = true;
		}
		void ToggleOff()
		{
			m_IsToggled = false;
		}
		void Update(const Data &data) {
			m_Level = data.level;
			m_IsActive = data.isActive;
			m_Cost = data.cost;
			m_Range = data.range;
			m_Name = data.name;
			m_Description = data.description;
			m_IconName = data.iconName;
		}

		/**
		* @deprecated
		**/
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
		/**
		* @deprecated
		**/
		void SaveState() override
		{
			m_PrevState =
			{
				m_Name,
				m_IconName,
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
		/**
		* @deprecated
		**/
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
		/**
		* @deprecated
		**/
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(m_SkillId) });
			result.push_back({ L"level", std::to_wstring(m_Level) });

			if (m_PrevState.isNewState)
			{
				result.push_back({ L"isActive", std::to_wstring(m_IsActive) });
			}

			if (m_PrevState.isNewState || m_Name != m_PrevState.name)
			{
				result.push_back({ L"name", m_Name });
			}
			if (m_PrevState.isNewState || m_IconName != m_PrevState.iconName)
			{
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

		Skill(const Data &data) :
			m_SkillId(data.skillId),
			m_Level(data.level),
			m_IsActive(data.isActive),
			m_Cost(data.cost),
			m_Range(data.range),
			m_Name(data.name),
			m_Description(data.description),
			m_IconName(data.iconName)
		{
			
		}

		/**
		* @deprecated
		**/
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

		virtual ~Skill() = default;

	private:
		/**
		* @deprecated
		**/
		struct GetState
		{
			std::wstring name = L"";
			std::wstring iconName = L"";
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
