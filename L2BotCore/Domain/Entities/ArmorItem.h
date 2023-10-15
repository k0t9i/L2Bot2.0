#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "BaseItem.h"
#include "../Enums/ArmorTypeEnum.h"
#include "../Enums/CrystalTypeEnum.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class ArmorItem : public BaseItem
	{
	public:
		void Update(
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::ArmorTypeEnum armorType,
			const Enums::CrystalTypeEnum crystalType,
			const uint32_t pDefense,
			const uint32_t mDefense,
			const std::wstring& setEffect,
			const std::wstring& addSetEffect,
			const std::wstring& enchantEffect
		) {
			BaseItem::Update(itemId, mana, name, iconName, description, weight);

			m_IsEquipped = isEquipped;
			m_EnchantLevel = enchantLevel;
			m_ArmorType = armorType;
			m_CrystalType = crystalType;
			m_PDefense = pDefense;
			m_MDefense = mDefense;
			m_SetEffect = setEffect;
			m_AddSetEffect = addSetEffect;
			m_EnchantEffect = enchantEffect;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				BaseItem::GetHash(),
				std::hash<bool>{}(m_IsEquipped),
				std::hash<uint16_t>{}(m_EnchantLevel),
				std::hash<Enums::ArmorTypeEnum>{}(m_ArmorType),
				std::hash<Enums::CrystalTypeEnum>{}(m_CrystalType),
				std::hash<uint32_t>{}(m_PDefense),
				std::hash<uint32_t>{}(m_MDefense),
				std::hash<std::wstring>{}(m_SetEffect),
				std::hash<std::wstring>{}(m_AddSetEffect),
				std::hash<std::wstring>{}(m_EnchantEffect)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			result.push_back({ L"armorType", std::to_wstring(static_cast<uint8_t>(m_ArmorType)) });
			result.push_back({ L"crystalType", std::to_wstring(static_cast<int8_t>(m_CrystalType)) });
			result.push_back({ L"setEffect", m_SetEffect });
			result.push_back({ L"addSetEffect", m_AddSetEffect });
			result.push_back({ L"enchantEffect", m_EnchantEffect });
			result.push_back({ L"isEquipped", std::to_wstring(m_IsEquipped) });
			result.push_back({ L"enchantLevel", std::to_wstring(m_EnchantLevel) });
			result.push_back({ L"pDefense", std::to_wstring(m_PDefense) });
			result.push_back({ L"mDefense", std::to_wstring(m_MDefense) });

			return result;
		}

		ArmorItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::ArmorTypeEnum armorType,
			const Enums::CrystalTypeEnum crystalType,
			const uint32_t pDefense,
			const uint32_t mDefense,
			const std::wstring& setEffect,
			const std::wstring& addSetEffect,
			const std::wstring& enchantEffect
		) :
			BaseItem
			(
				objectId,
				itemId,
				mana,
				name,
				iconName,
				description,
				weight,
				Enums::ItemTypeEnum::armor
			),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_ArmorType(armorType),
			m_CrystalType(crystalType),
			m_PDefense(pDefense),
			m_MDefense(mDefense),
			m_SetEffect(setEffect),
			m_AddSetEffect(addSetEffect),
			m_EnchantEffect(enchantEffect)
		{
		}

		ArmorItem() = default;
		virtual ~ArmorItem() = default;

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::ArmorTypeEnum m_ArmorType = Enums::ArmorTypeEnum::none;
		Enums::CrystalTypeEnum m_CrystalType = Enums::CrystalTypeEnum::none;
		uint32_t m_PDefense = 0;
		uint32_t m_MDefense = 0;
		std::wstring m_SetEffect = L"";
		std::wstring m_AddSetEffect = L"";
		std::wstring m_EnchantEffect = L"";
	};
}
