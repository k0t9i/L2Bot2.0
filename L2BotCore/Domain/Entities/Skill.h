#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "EntityInterface.h"
#include "../Helpers/HashCombiner.h"

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
		void Update(
			const uint8_t level,
			const bool isActive,
			const uint8_t cost,
			const int16_t range,
			const std::wstring& name,
			const std::wstring& description,
			const std::wstring& iconName
		) {
			m_Level = level;
			m_IsActive = isActive;
			m_Cost = cost;
			m_Range = range;
			m_Name = name;
			m_Description = description;
			m_IconName = iconName;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint32_t>{}(m_SkillId),
				std::hash<uint8_t>{}(m_Level),
				std::hash<bool>{}(m_IsActive),
				std::hash<uint8_t>{}(m_Cost),
				std::hash<int16_t>{}(m_Range),
				std::hash<std::wstring>{}(m_Name),
				std::hash<std::wstring>{}(m_Description),
				std::hash<std::wstring>{}(m_IconName),
				std::hash<bool>{}(m_IsToggled),
				std::hash<bool>{}(m_IsCasting),
				std::hash<bool>{}(m_IsReloading)
			});
		}
		const std::string GetEntityName() const override
		{
			return "skill";
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(m_SkillId) });
			result.push_back({ L"level", std::to_wstring(m_Level) });
			result.push_back({ L"isActive", std::to_wstring(m_IsActive) });
			result.push_back({ L"name", m_Name });
			result.push_back({ L"iconName", m_IconName });
			result.push_back({ L"description", m_Description });
			result.push_back({ L"cost", std::to_wstring(m_Cost) });
			result.push_back({ L"range", std::to_wstring(m_Range) });
			result.push_back({ L"isToggled", std::to_wstring(m_IsToggled) });
			result.push_back({ L"isCasting", std::to_wstring(m_IsCasting) });
			result.push_back({ L"isReloading", std::to_wstring(m_IsReloading) });
			result.push_back({ L"isReadyToUse", std::to_wstring(IsReadyToUse()) });

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
			const std::wstring& iconName
		) :
			m_SkillId(skillId),
			m_Level(level),
			m_IsActive(isActive),
			m_Cost(cost),
			m_Range(range),
			m_Name(name),
			m_Description(description),
			m_IconName(iconName)
		{
			
		}

		Skill() = default;
		virtual ~Skill() = default;

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
	};
}
