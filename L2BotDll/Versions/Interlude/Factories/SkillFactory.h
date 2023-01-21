#pragma once

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

		Entities::Skill* Create(const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto data = m_L2GameData.GetMSData(skillId, level);

			const auto cost = data ? data->mpCost : 0;
			const auto range = data ? data->range : 0;
			const auto name = data ? data->name : L"";
			const auto description = data ? data->description : L"";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;

			return new Entities::Skill
			{
				skillId,
				static_cast<uint8_t>(level),
				isActive != 1,
				static_cast<uint8_t>(cost),
				static_cast<int16_t>(range),
				ConvertFromWideChar(name),
				ConvertFromWideChar(description),
				iconEntry ? ConvertFromWideChar(iconEntry->value) : "",
				false,
				false,
				false
			};
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}