#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/EtcItem.h"
#include "Domain/Entities/ArmorItem.h"
#include "Domain/Entities/WeaponItem.h"
#include "Domain/Entities/ShieldItem.h"
#include "../../../DTO/ItemData.h"
#include "../Helpers/EnchantHelper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ItemFactory
	{
	public:
		ItemFactory(const L2GameDataWrapper& l2GameData, const FName& fName, const EnchantHelper& enchantHelper) :
			m_L2GameData(l2GameData),
			m_FName(fName),
			m_EnchantHelper(enchantHelper)
		{
		}

		ItemFactory() = delete;
		virtual ~ItemFactory() = default;

		std::unique_ptr<Entities::BaseItem> Create(const ItemData& itemInfo) const
		{
			const auto data = m_L2GameData.GetItemData(itemInfo.itemId);

			const auto nameEntry = data ? m_FName.GetEntry(data->nameIndex) : nullptr;
			const auto name = nameEntry ? ConvertFromWideChar(nameEntry->value) : "";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;
			const auto icon = iconEntry ? ConvertFromWideChar(iconEntry->value) : "";
			const auto description = data && data->description ? ConvertFromWideChar(data->description) : "";

			if (data)
			{
				switch (data->dataType)
				{
				case L2::ItemDataType::ARMOR:
					return CreateArmor(itemInfo, data, name, icon, description);
				case L2::ItemDataType::WEAPON:
					return CreateWeaponOrShield(itemInfo, data, name, icon, description);
				}
			}

			return CreateEtc(itemInfo, data, name, icon, description);
		}

		std::unique_ptr<Entities::BaseItem> CreateFromPointer(const Entities::BaseItem* other) const
		{
			{
				const auto object = dynamic_cast<const Entities::EtcItem*>(other);
				if (object)
				{
					return std::make_unique<Entities::EtcItem>(object);
				}
			}
			{
				const auto object = dynamic_cast<const Entities::ArmorItem*>(other);
				if (object)
				{
					return std::make_unique<Entities::ArmorItem>(object);
				}
			}
			{
				const auto object = dynamic_cast<const Entities::WeaponItem*>(other);
				if (object)
				{
					return std::make_unique<Entities::WeaponItem>(object);
				}
			}
			{
				const auto object = dynamic_cast<const Entities::ShieldItem*>(other);
				if (object)
				{
					return std::make_unique<Entities::ShieldItem>(object);
				}
			}

			return std::make_unique<Entities::BaseItem>(other);
		}

	private:
		std::unique_ptr<Entities::BaseItem> CreateEtc(
			const ItemData& itemInfo,
			const FL2ItemDataBase* itemData,
			const std::string& name,
			const std::string& icon,
			const std::string& description
		) const
		{
			return std::make_unique<Entities::EtcItem>(
				itemInfo.objectId,
				itemInfo.itemId,
				itemInfo.mana,
				name,
				icon,
				description,
				itemData ? itemData->weight : 0,
				itemInfo.amount,
				itemInfo.isQuest
			);
		}

		std::unique_ptr<Entities::BaseItem> CreateArmor(
			const ItemData& itemInfo,
			const FL2ItemDataBase* itemData,
			const std::string& name,
			const std::string& icon,
			const std::string& description
		) const
		{
			const auto casted = static_cast<const FL2ArmorItemData*>(itemData);

			const auto setEffect = casted && casted->setEffect ? ConvertFromWideChar(casted->setEffect) : "";
			const auto addSetEffect = casted && casted->setEffect ? ConvertFromWideChar(casted->setEffect) : "";
			const auto enchantEffect = casted && casted->enchantEffect ? ConvertFromWideChar(casted->enchantEffect) : "";

			return std::make_unique<Entities::ArmorItem>(
				itemInfo.objectId,
				itemInfo.itemId,
				itemInfo.mana,
				name,
				icon,
				description,
				itemData ? itemData->weight : 0,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				casted ? static_cast<Enums::ArmorTypeEnum>(casted->armorType) : Enums::ArmorTypeEnum::none,
				casted ? static_cast<Enums::CrystalTypeEnum>(casted->crystalType) : Enums::CrystalTypeEnum::none,
				m_EnchantHelper.GetDefenseEnchantValue(casted ? casted->pDefense : 0, itemInfo.enchantLevel),
				m_EnchantHelper.GetDefenseEnchantValue(casted ? casted->mDefense : 0, itemInfo.enchantLevel),
				setEffect,
				addSetEffect,
				enchantEffect
			);
		}

		std::unique_ptr<Entities::BaseItem> CreateWeaponOrShield(
			const ItemData& itemInfo,
			const FL2ItemDataBase* itemData,
			const std::string& name,
			const std::string& icon,
			const std::string& description
		) const
		{
			const auto casted = static_cast<const FL2WeaponItemData*>(itemData);

			if (casted->weaponType != L2::WeaponType::SHIELD)
			{
				return std::make_unique<Entities::WeaponItem>(
					itemInfo.objectId,
					itemInfo.itemId,
					itemInfo.mana,
					name,
					icon,
					description,
					itemData ? itemData->weight : 0,
					itemInfo.isEquipped > 0,
					itemInfo.enchantLevel,
					casted ? static_cast<Enums::WeaponTypeEnum>(casted->weaponType) : Enums::WeaponTypeEnum::none,
					casted ? static_cast<Enums::CrystalTypeEnum>(casted->crystalType) : Enums::CrystalTypeEnum::none,
					casted ? casted->rndDamage : 0,
					m_EnchantHelper.GetPAttackEnchantValue(casted->weaponType, itemInfo.isTwoHanded, casted->crystalType, casted ? casted->pAttack : 0, itemInfo.enchantLevel),
					m_EnchantHelper.GetMAttackEnchantValue(casted->crystalType, casted ? casted->mAttack : 0, itemInfo.enchantLevel),
					casted ? casted->critical : 0,
					casted ? casted->hitModify : 0,
					casted ? casted->atkSpd : 0,
					casted ? casted->mpConsume : 0,
					casted ? casted->soulshotCount : 0,
					casted ? casted->spiritshotCount : 0
				);
			}

			return std::make_unique<Entities::ShieldItem>(
				itemInfo.objectId,
				itemInfo.itemId,
				itemInfo.mana,
				name,
				icon,
				description,
				itemData ? itemData->weight : 0,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				casted ? static_cast<Enums::CrystalTypeEnum>(casted->crystalType) : Enums::CrystalTypeEnum::none,
				casted ? casted->shieldEvasion : 0,
				m_EnchantHelper.GetDefenseEnchantValue(casted ? casted->shieldPdef : 0, itemInfo.enchantLevel),
				casted ? casted->shieldDefRate : 0
			);
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
		const EnchantHelper& m_EnchantHelper;
	};
}