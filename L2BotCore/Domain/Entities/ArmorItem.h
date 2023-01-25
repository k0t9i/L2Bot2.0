#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"
#include "../Enums/ArmorTypeEnum.h"
#include "../Enums/CrystalTypeEnum.h"

namespace L2Bot::Domain::Entities
{
	class ArmorItem : public BaseItem
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const ArmorItem* casted = static_cast<const ArmorItem*>(other);

			BaseItem::Update(other);

			m_IsEquipped = casted->m_IsEquipped;
			m_EnchantLevel = casted->m_EnchantLevel;
			m_ArmorType = casted->m_ArmorType;
			m_CrystalType = casted->m_CrystalType;
			m_PDefense = casted->m_PDefense;
			m_MDefense = casted->m_MDefense;
			m_SetEffect = casted->m_SetEffect;
			m_AddSetEffect = casted->m_AddSetEffect;
			m_EnchantEffect = casted->m_EnchantEffect;
		}
		void SaveState() override
		{
			BaseItem::SaveState();
			m_PrevState =
			{
				m_IsEquipped,
				m_EnchantLevel,
				m_PDefense,
				m_MDefense,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const ArmorItem* casted = static_cast<const ArmorItem*>(other);
			return BaseItem::IsEqual(other) &&
				m_IsEquipped == casted->m_IsEquipped &&
				m_EnchantLevel == casted->m_EnchantLevel &&
				m_ArmorType == casted->m_ArmorType &&
				m_CrystalType == casted->m_CrystalType &&
				m_PDefense == casted->m_PDefense &&
				m_MDefense == casted->m_MDefense &&
				m_SetEffect == casted->m_SetEffect &&
				m_AddSetEffect == casted->m_AddSetEffect &&
				m_EnchantEffect == casted->m_EnchantEffect;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			if (m_PrevState.isNewState)
			{
				result.push_back({ "armorType", std::to_string(static_cast<uint8_t>(m_ArmorType)) });
				result.push_back({ "crystalType", std::to_string(static_cast<int8_t>(m_CrystalType)) });
				result.push_back({ "setEffect", m_SetEffect });
				result.push_back({ "addSetEffect", m_AddSetEffect });
				result.push_back({ "enchantEffect", m_EnchantEffect });
			}

			if (m_PrevState.isNewState || m_IsEquipped != m_PrevState.isEquipped)
			{
				result.push_back({ "isEquipped", std::to_string(m_IsEquipped) });
			}
			if (m_PrevState.isNewState || m_EnchantLevel != m_PrevState.enchantLevel)
			{
				result.push_back({ "enchantLevel", std::to_string(m_EnchantLevel) });
			}
			if (m_PrevState.isNewState || m_PDefense != m_PrevState.pDefense)
			{
				result.push_back({ "pDefense", std::to_string(m_PDefense) });
			}
			if (m_PrevState.isNewState || m_MDefense != m_PrevState.mDefense)
			{
				result.push_back({ "mDefense", std::to_string(m_MDefense) });
			}

			return result;
		}

		ArmorItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::string& name,
			const std::string& iconName,
			const std::string& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::ArmorTypeEnum armorType,
			const Enums::CrystalTypeEnum crystalType,
			const uint32_t pDefense,
			const uint32_t mDefense,
			const std::string& setEffect,
			const std::string& addSetEffect,
			const std::string& enchantEffect
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

		ArmorItem(const ArmorItem* other) :
			BaseItem(other),
			m_IsEquipped(other->m_IsEquipped),
			m_EnchantLevel(other->m_EnchantLevel),
			m_ArmorType(other->m_ArmorType),
			m_CrystalType(other->m_CrystalType),
			m_PDefense(other->m_PDefense),
			m_MDefense(other->m_MDefense),
			m_SetEffect(other->m_SetEffect),
			m_AddSetEffect(other->m_AddSetEffect),
			m_EnchantEffect(other->m_EnchantEffect)
		{
		}

		ArmorItem() = default;
		virtual ~ArmorItem() = default;

	private:
		struct GetState
		{
			bool isEquipped = 0;
			uint16_t enchantLevel = 0;
			uint32_t pDefense = 0;
			uint32_t mDefense = 0;

			bool isNewState = true;
		};

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::ArmorTypeEnum m_ArmorType = Enums::ArmorTypeEnum::none;
		Enums::CrystalTypeEnum m_CrystalType = Enums::CrystalTypeEnum::none;
		uint32_t m_PDefense = 0;
		uint32_t m_MDefense = 0;
		std::string m_SetEffect = "";
		std::string m_AddSetEffect = "";
		std::string m_EnchantEffect = "";
		GetState m_PrevState = GetState();
	};
}
