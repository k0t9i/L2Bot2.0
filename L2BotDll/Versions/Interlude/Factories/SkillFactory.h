#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Skill.h"
#include "Domain/Exceptions.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class SkillFactory
	{
	private:
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
		SkillFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		SkillFactory() = delete;
		virtual ~SkillFactory() = default;

		std::shared_ptr<Entities::Skill> Create(const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto& data = GetData(skillId, level, isActive);

			return std::make_shared<Entities::Skill>(
				data.skillId,
				data.level,
				data.isActive,
				data.cost,
				data.range,
				data.name,
				data.description,
				data.iconName
			);
		}

		void Update(std::shared_ptr<Entities::Skill>& skill, const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto& data = GetData(skillId, level, isActive);

			skill->Update(
				data.level,
				data.isActive,
				data.cost,
				data.range,
				data.name,
				data.description,
				data.iconName
			);
		}

	private:
		const Data GetData(const uint32_t skillId, const uint32_t level, const uint32_t isActive) const
		{
			const auto data = m_L2GameData.GetMSData(skillId, level);
			if (!data) {
				throw RuntimeException(std::format(L"cannot load MSData for skill {}", skillId));
			}

			const auto iconEntry = m_FName.GetEntry(data->iconNameIndex);

			return {
				skillId,
				static_cast<uint8_t>(level),
				isActive != 1,
				static_cast<uint8_t>(data->mpCost),
				static_cast<int16_t>(data->range),
				data->name ? std::wstring(data->name) : L"",
				data->description ? std::wstring(data->description) : L"",
				iconEntry ? std::wstring(iconEntry->value) : L"",
			};
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}