#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "../DTO/Skill.h"
#include "../Serializers/Serializable.h"
#include "../Serializers/Node.h"

namespace L2Bot::Domain::ValueObjects
{
	class Skill : public Serializers::Serializable
	{
	public:
		const uint32_t GetId() const
		{
			return m_SkillId;
		}
		const bool IsReadyToUse() const
		{
			return !m_IsCasting && !m_IsReloading;
		}
		void UpdateFromDTO(const DTO::Skill* dto)
		{
			SaveState();

			m_SkillId = dto->skillId;
			m_Level = dto->level;
			m_IsActive = dto->isActive;
			m_Cost = dto->cost;
			m_Range = dto->range;
			m_Name = dto->name;
			m_Description = dto->description;
			m_IconName = dto->iconName;
			m_IsToggled = dto->isToggled;
			m_IsCasting = dto->isCasting;
			m_IsReloading = dto->isReloading;
		}
		void SaveState()
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
		const static Skill CreateFromDTO(const DTO::Skill& dto)
		{
			return Skill(
				dto.skillId,
				dto.level,
				dto.isActive,
				dto.cost,
				dto.range,
				dto.name,
				dto.description,
				dto.iconName,
				dto.isToggled,
				dto.isCasting,
				dto.isReloading
			);
		}
		const bool IsEqual(const DTO::Skill* dto) const
		{
			return m_SkillId == dto->skillId &&
				m_Level == dto->level &&
				m_IsActive == dto->isActive &&
				m_Cost == dto->cost &&
				m_Range == dto->range &&
				m_Name == dto->name &&
				m_Description == dto->description &&
				m_IconName == dto->iconName &&
				m_IsToggled == dto->isToggled &&
				m_IsCasting == dto->isCasting &&
				m_IsReloading == dto->isReloading;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ "skillId", std::to_string(m_SkillId) });
			result.push_back({ "level", std::to_string(m_Level) });

			if (m_PrevState.isNewState)
			{
				result.push_back({ "isActive", std::to_string(m_IsActive) });
				result.push_back({ "name", m_Name });
				result.push_back({ "iconName", m_IconName });
			}

			if (m_PrevState.isNewState || m_Description != m_PrevState.description)
			{
				result.push_back({ "description", m_Description });
			}
			if (m_PrevState.isNewState || m_Cost != m_PrevState.cost)
			{
				result.push_back({ "cost", std::to_string(m_Cost) });
			}
			if (m_PrevState.isNewState || m_Range != m_PrevState.range)
			{
				result.push_back({ "range", std::to_string(m_Range) });
			}
			if (m_PrevState.isNewState || m_IsToggled != m_PrevState.isToggled)
			{
				result.push_back({ "isToggled", std::to_string(m_IsToggled) });
			}
			if (m_PrevState.isNewState || m_IsCasting != m_PrevState.isCasting)
			{
				result.push_back({ "isCasting", std::to_string(m_IsCasting) });
			}
			if (m_PrevState.isNewState || m_IsReloading != m_PrevState.isReloading)
			{
				result.push_back({ "isReloading", std::to_string(m_IsReloading) });
			}
			if (m_PrevState.isNewState || IsReadyToUse() != m_PrevState.isReadyToUse)
			{
				result.push_back({ "isReadyToUse", std::to_string(IsReadyToUse()) });
			}

			return result;
		}

		Skill(
			const uint32_t skillId,
			const uint8_t level,
			const bool isActive,
			const uint8_t cost,
			const int16_t range,
			const std::string& name,
			const std::string& description,
			const std::string& iconName,
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

		Skill() = default;
		virtual ~Skill() = default;

	private:
		struct State
		{
			uint8_t cost = 0;
			int16_t range = 0;
			std::string description = "";
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
		std::string m_Name = "";
		std::string m_Description = "";
		std::string m_IconName = "";
		bool m_IsToggled = false;
		bool m_IsCasting = false;
		bool m_IsReloading = false;
		State m_PrevState = State();
	};
}
