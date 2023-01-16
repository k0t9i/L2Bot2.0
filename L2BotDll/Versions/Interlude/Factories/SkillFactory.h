#pragma once

#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"

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

		const DTO::Skill Create(const DTO::Skill source, const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto data = m_L2GameData.GetMSData(skillId, level);

			const auto cost = data ? data->mpCost : 0;
			const auto range = data ? data->range : 0;
			const auto name = data ? data->name : L"";
			const auto description = data ? data->description : L"";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;

			return DTO::Skill
			{
				skillId,
				static_cast<uint8_t>(level),
				isActive != 1,
				static_cast<uint8_t>(cost),
				static_cast<int16_t>(range),
				ConvertFromWideChar(name),
				ConvertFromWideChar(description),
				iconEntry ? ConvertFromWideChar(iconEntry->value) : "",
				source.isToggled,
				source.isCasting,
				source.isReloading
			};
		}

		const DTO::Skill UpdateToggle(const DTO::Skill source, const bool isToggled) const
		{
			return DTO::Skill
			{
				source.skillId,
				source.level,
				source.isActive,
				source.cost,
				source.range,
				source.name,
				source.description,
				source.iconName,
				isToggled,
				source.isCasting,
				source.isReloading
			};
		}

		const DTO::Skill UpdateCastingState(const DTO::Skill source, const bool isCasting) const
		{
			return DTO::Skill
			{
				source.skillId,
				source.level,
				source.isActive,
				source.cost,
				source.range,
				source.name,
				source.description,
				source.iconName,
				source.isToggled,
				isCasting,
				source.isReloading
			};
		}

		const DTO::Skill UpdateReloadingState(const DTO::Skill source, const bool isReloading) const
		{
			return DTO::Skill
			{
				source.skillId,
				source.level,
				source.isActive,
				source.cost,
				source.range,
				source.name,
				source.description,
				source.iconName,
				source.isToggled,
				source.isCasting,
				isReloading
			};
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}