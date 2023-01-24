#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/AbnormalEffect.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class AbnormalEffectFactory
	{
	public:
		AbnormalEffectFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		AbnormalEffectFactory() = delete;
		virtual ~AbnormalEffectFactory() = default;

		std::unique_ptr<Entities::AbnormalEffect> Create(const uint32_t skillId, const uint32_t level) const
		{
			const auto data = m_L2GameData.GetMSData(skillId, level);

			const auto name = data && data->name ? data->name : L"";
			const auto description = data && data->description ? data->description : L"";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;

			return std::make_unique<Entities::AbnormalEffect>(
				skillId,
				static_cast<uint8_t>(level),
				ConvertFromWideChar(name),
				ConvertFromWideChar(description),
				iconEntry ? ConvertFromWideChar(iconEntry->value) : ""
			);
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}