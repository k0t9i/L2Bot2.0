#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Skill.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class SkillFactory
	{
	public:
		SkillFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		SkillFactory() = delete;
		virtual ~SkillFactory() = default;

		std::shared_ptr<Entities::Skill> Create(const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			return std::make_shared<Entities::Skill>(BuildSkillData(skillId, level, isActive));
		}

		void Update(std::shared_ptr<Entities::Skill>& skill, const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			skill->Update(BuildSkillData(skillId, level, isActive));
		}

	private:
		const Entities::Skill::Data BuildSkillData(const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto data = m_L2GameData.GetMSData(skillId, level);

			const auto cost = data ? data->mpCost : 0;
			const auto range = data ? data->range : 0;
			const auto name = data && data->name ? data->name : L"";
			const auto description = data && data->description ? data->description : L"";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;

			return {
				skillId,
				static_cast<uint8_t>(level),
				isActive != 1,
				static_cast<uint8_t>(cost),
				static_cast<int16_t>(range),
				std::wstring(name),
				std::wstring(description),
				iconEntry ? std::wstring(iconEntry->value) : L"",
			};
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}