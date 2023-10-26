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
	private:
		struct Data
		{
			uint32_t skillId = 0;
			uint8_t level = 0;
			bool isActive = false;
			uint8_t cost = 0;
			int16_t range = 0;
			std::wstring name = L"";
			std::wstring description = L"";
			std::wstring iconName = L"";
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

			if (data.skillId == 0) {
				return nullptr;
			}

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

			if (data.skillId == 0) {
				return;
			}

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
				return Data();
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