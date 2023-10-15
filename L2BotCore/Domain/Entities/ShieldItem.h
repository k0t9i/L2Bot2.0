#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "BaseItem.h"
#include "../Enums/CrystalTypeEnum.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class ShieldItem : public BaseItem
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
			const Enums::CrystalTypeEnum crystalType,
			const int16_t evasion,
			const uint32_t pDefense,
			const uint16_t defRate
		) {
			BaseItem::Update(itemId, mana, name, iconName, description, weight);

			m_IsEquipped = isEquipped;
			m_EnchantLevel = enchantLevel;
			m_CrystalType = crystalType;
			m_Evasion = evasion;
			m_PDefense = pDefense;
			m_DefRate = defRate;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				BaseItem::GetHash(),
				std::hash<bool>{}(m_IsEquipped),
				std::hash<uint16_t>{}(m_EnchantLevel),
				std::hash<Enums::CrystalTypeEnum>{}(m_CrystalType),
				std::hash<int16_t>{}(m_Evasion),
				std::hash<uint32_t>{}(m_PDefense),
				std::hash<uint16_t>{}(m_DefRate)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			result.push_back({ L"crystalType", std::to_wstring(static_cast<int8_t>(m_CrystalType)) });
			result.push_back({ L"evasion", std::to_wstring(m_Evasion) });
			result.push_back({ L"defRate", std::to_wstring(m_DefRate) });
			result.push_back({ L"isEquipped", std::to_wstring(m_IsEquipped) });
			result.push_back({ L"enchantLevel", std::to_wstring(m_EnchantLevel) });
			result.push_back({ L"pDefense", std::to_wstring(m_PDefense) });

			return result;
		}

		ShieldItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::CrystalTypeEnum crystalType,
			const int16_t evasion,
			const uint32_t pDefense,
			const uint16_t defRate
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
			Enums::ItemTypeEnum::shield
		),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_CrystalType(crystalType),
			m_Evasion(evasion),
			m_PDefense(pDefense),
			m_DefRate(defRate)
		{
		}

		ShieldItem() = default;
		virtual ~ShieldItem() = default;

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::CrystalTypeEnum m_CrystalType = Enums::CrystalTypeEnum::none;
		int16_t m_Evasion = 0;
		uint32_t m_PDefense = 0;
		uint16_t m_DefRate = 0;
	};
}
