#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"
#include "../Enums/CrystalType.h"

namespace L2Bot::Domain::Entities
{
	class ShieldItem : public BaseItem
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const ShieldItem* casted = static_cast<const ShieldItem*>(other);

			BaseItem::Update(other);

			m_IsEquipped = casted->m_IsEquipped;
			m_EnchantLevel = casted->m_EnchantLevel;
			m_CrystalType = casted->m_CrystalType;
			m_Evasion = casted->m_Evasion;
			m_PDef = casted->m_PDef;
			m_DefRate = casted->m_DefRate;
		}
		void SaveState() override
		{
			BaseItem::SaveState();
			m_PrevState =
			{
				m_IsEquipped,
				m_EnchantLevel,
				m_PDef,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const ShieldItem* casted = static_cast<const ShieldItem*>(other);
			return BaseItem::IsEqual(other) &&
				m_IsEquipped == casted->m_IsEquipped &&
				m_EnchantLevel == casted->m_EnchantLevel &&
				m_CrystalType == casted->m_CrystalType &&
				m_Evasion == casted->m_Evasion &&
				m_PDef == casted->m_PDef &&
				m_DefRate == casted->m_DefRate;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			if (m_PrevState.isNewState)
			{
				result.push_back({ "crystalType", std::to_string(static_cast<int8_t>(m_CrystalType)) });
				result.push_back({ "evasion", std::to_string(m_Evasion) });
				result.push_back({ "defRate", std::to_string(m_DefRate) });
			}

			if (m_PrevState.isNewState || m_IsEquipped != m_PrevState.isEquipped)
			{
				result.push_back({ "isEquipped", std::to_string(m_IsEquipped) });
			}
			if (m_PrevState.isNewState || m_EnchantLevel != m_PrevState.enchantLevel)
			{
				result.push_back({ "enchantLevel", std::to_string(m_EnchantLevel) });
			}
			if (m_PrevState.isNewState || m_PDef != m_PrevState.pDef)
			{
				result.push_back({ "pDef", std::to_string(m_PDef) });
			}

			return result;
		}

		ShieldItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::string& name,
			const std::string& iconName,
			const std::string& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::CrystalType crystalType,
			const int16_t evasion,
			const uint32_t pDef,
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
			Enums::ItemType::shield
		),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_CrystalType(crystalType),
			m_Evasion(evasion),
			m_PDef(pDef),
			m_DefRate(defRate)
		{
		}

		ShieldItem(const ShieldItem* other) :
			BaseItem(other),
			m_IsEquipped(other->m_IsEquipped),
			m_EnchantLevel(other->m_EnchantLevel),
			m_CrystalType(other->m_CrystalType),
			m_Evasion(other->m_Evasion),
			m_PDef(other->m_PDef),
			m_DefRate(other->m_DefRate)

		{
		}

		ShieldItem() = default;
		virtual ~ShieldItem() = default;

	private:
		struct GetState
		{
			bool isEquipped = 0;
			uint16_t enchantLevel = 0;
			uint32_t pDef = 0;

			bool isNewState = true;
		};

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::CrystalType m_CrystalType = Enums::CrystalType::none;
		int16_t m_Evasion = 0;
		uint32_t m_PDef = 0;
		uint16_t m_DefRate = 0;

		GetState m_PrevState = GetState();
	};
}
