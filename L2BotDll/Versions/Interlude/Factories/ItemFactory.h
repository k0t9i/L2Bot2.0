#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/BaseItem.h"
#include "../../../DTO/ItemData.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ItemFactory
	{
	public:
		ItemFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		ItemFactory() = delete;
		virtual ~ItemFactory() = default;

		std::unique_ptr<Entities::BaseItem> Create(const ItemData& itemInfo) const
		{
			const auto data = m_L2GameData.GetItemData(itemInfo.itemId);

			const auto nameEntry = data ? m_FName.GetEntry(data->nameIndex) : nullptr;
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;
			const auto description = data && data->description ? data->description : L"";

			return std::make_unique<Entities::BaseItem>(
				itemInfo.itemId,
				itemInfo.amount,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				itemInfo.mana,
				nameEntry ? ConvertFromWideChar(nameEntry->value) : "",
				iconEntry ? ConvertFromWideChar(iconEntry->value) : "",
				ConvertFromWideChar(description),
				data ? data->weight : 0
			);
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}