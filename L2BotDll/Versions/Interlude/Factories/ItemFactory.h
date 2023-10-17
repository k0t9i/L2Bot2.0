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
#include "Domain/DTO/ItemData.h"
#include "../Helpers/EnchantHelper.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ItemFactory
	{
	private:
		struct BaseData
		{
			uint32_t objectId;
			uint32_t itemId;
			int32_t mana;
			std::wstring name;
			std::wstring iconName;
			std::wstring description;
			uint16_t weight;
		};

		struct EtcData : public BaseData
		{
			uint32_t amount;
			bool isQuest;
		};

		struct ArmorData : public BaseData
		{
			bool isEquipped;
			uint16_t enchantLevel;
			Enums::ArmorTypeEnum armorType;
			Enums::CrystalTypeEnum crystalType;
			uint32_t pDefense;
			uint32_t mDefense;
			std::wstring setEffect;
			std::wstring addSetEffect;
			std::wstring enchantEffect;
		};

		struct ShieldData : public BaseData
		{
			bool isEquipped;
			uint16_t enchantLevel;
			Enums::CrystalTypeEnum crystalType;
			int16_t evasion;
			uint32_t pDefense;
			uint16_t defRate;
		};

		struct WeaponData : public BaseData
		{
			bool isEquipped;
			uint16_t enchantLevel;
			Enums::WeaponTypeEnum weaponType;
			Enums::CrystalTypeEnum crystalType;
			uint8_t rndDamage;
			uint32_t pAttack;
			uint32_t mAttack;
			uint16_t critical;
			int8_t hitModify;
			uint16_t atkSpd;
			uint8_t mpConsume;
			uint8_t soulshotCount;
			uint8_t spiritshotCount;
		};

	public:
		ItemFactory(const L2GameDataWrapper& l2GameData, const FName& fName, const EnchantHelper& enchantHelper) :
			m_L2GameData(l2GameData),
			m_FName(fName),
			m_EnchantHelper(enchantHelper)
		{
		}

		ItemFactory() = delete;
		virtual ~ItemFactory() = default;

		std::shared_ptr<Entities::BaseItem> Create(const DTO::ItemData& itemInfo) const
		{
			//FIXME during first start data may be undefined
			const auto data = GetItemData(itemInfo.itemId);

			if (data)
			{
				switch (data->dataType)
				{
				case L2::ItemDataType::ARMOR:
					return CreateArmor(itemInfo);
				case L2::ItemDataType::WEAPON:
					return CreateWeaponOrShield(itemInfo);
				}

				return CreateEtc(itemInfo);
			}

			return nullptr;
		}

		void Update(std::shared_ptr<Entities::BaseItem>& item, const DTO::ItemData& itemInfo) const
		{
			//FIXME during first start data may be undefined
			const auto data = GetItemData(itemInfo.itemId);

			if (data)
			{
				switch (data->dataType)
				{
				case L2::ItemDataType::ARMOR:
					UpdateArmor(item, itemInfo);
					return;
				case L2::ItemDataType::WEAPON:
					UpdateWeaponOrShield(item, itemInfo);
					return;
				}
			}

			UpdateEtc(item, itemInfo);
		}

	private:
		std::shared_ptr<Entities::BaseItem> CreateEtc(const DTO::ItemData& itemInfo) const
		{
			const auto& data = GetEtcData(itemInfo);

			return std::make_shared<Entities::EtcItem>(
				data.objectId,
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.amount,
				data.isQuest
			);
		}

		void UpdateEtc(std::shared_ptr<Entities::BaseItem> &item, const DTO::ItemData& itemInfo) const
		{
			auto etcItem = std::dynamic_pointer_cast<Entities::EtcItem>(item);

			const auto& data = GetEtcData(itemInfo);

			etcItem->Update(
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.amount,
				data.isQuest
			);
		}

		std::shared_ptr<Entities::BaseItem> CreateArmor(const DTO::ItemData& itemInfo) const
		{
			const auto& data = GetArmorData(itemInfo);

			return std::make_shared<Entities::ArmorItem>(
				data.objectId,
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.isEquipped,
				data.enchantLevel,
				data.armorType,
				data.crystalType,
				data.pDefense,
				data.mDefense,
				data.setEffect,
				data.addSetEffect,
				data.enchantEffect
			);
		}

		void UpdateArmor(std::shared_ptr<Entities::BaseItem>& item, const DTO::ItemData& itemInfo) const
		{
			auto armorItem = std::dynamic_pointer_cast<Entities::ArmorItem>(item);

			const auto& data = GetArmorData(itemInfo);

			armorItem->Update(
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.isEquipped,
				data.enchantLevel,
				data.armorType,
				data.crystalType,
				data.pDefense,
				data.mDefense,
				data.setEffect,
				data.addSetEffect,
				data.enchantEffect
			);
		}

		std::shared_ptr<Entities::BaseItem> CreateWeaponOrShield(const DTO::ItemData& itemInfo) const
		{
			const auto itemData = static_cast<const FL2WeaponItemData*>(GetItemData(itemInfo.itemId));

			if (itemData->weaponType != L2::WeaponType::SHIELD)
			{
				const auto& data = GetWeaponData(itemInfo, itemData);

				return std::make_shared<Entities::WeaponItem>(
					data.objectId,
					data.itemId,
					data.mana,
					data.name,
					data.iconName,
					data.description,
					data.weight,
					data.isEquipped,
					data.enchantLevel,
					data.weaponType,
					data.crystalType,
					data.rndDamage,
					data.pAttack,
					data.mAttack,
					data.critical,
					data.hitModify,
					data.atkSpd,
					data.mpConsume,
					data.soulshotCount,
					data.spiritshotCount
				);
			}

			const auto& data = GetShieldData(itemInfo, itemData);

			return std::make_shared<Entities::ShieldItem>(
				data.objectId,
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.isEquipped,
				data.enchantLevel,
				data.crystalType,
				data.evasion,
				data.pDefense,
				data.defRate
			);
		}

		void UpdateWeaponOrShield(std::shared_ptr<Entities::BaseItem>& item, const DTO::ItemData& itemInfo) const
		{
			const auto itemData = static_cast<const FL2WeaponItemData*>(GetItemData(itemInfo.itemId));

			if (itemData->weaponType != L2::WeaponType::SHIELD)
			{
				auto weaponItem = std::dynamic_pointer_cast<Entities::WeaponItem>(item);
				const auto& data = GetWeaponData(itemInfo, itemData);

				weaponItem->Update(
					data.itemId,
					data.mana,
					data.name,
					data.iconName,
					data.description,
					data.weight,
					data.isEquipped,
					data.enchantLevel,
					data.weaponType,
					data.crystalType,
					data.rndDamage,
					data.pAttack,
					data.mAttack,
					data.critical,
					data.hitModify,
					data.atkSpd,
					data.mpConsume,
					data.soulshotCount,
					data.spiritshotCount
				);

				return;
			}

			auto shieldItem = std::dynamic_pointer_cast<Entities::ShieldItem>(item);
			const auto& data = GetShieldData(itemInfo, itemData);

			shieldItem->Update(
				data.itemId,
				data.mana,
				data.name,
				data.iconName,
				data.description,
				data.weight,
				data.isEquipped,
				data.enchantLevel,
				data.crystalType,
				data.evasion,
				data.pDefense,
				data.defRate
			);
		}

		const BaseData GetBaseData(const DTO::ItemData& itemInfo) const
		{
			const auto data = GetItemData(itemInfo.itemId);

			const auto nameEntry = data ? m_FName.GetEntry(data->nameIndex) : nullptr;
			const auto name = nameEntry ? std::wstring(nameEntry->value) : L"";
			const auto iconEntry = data ? m_FName.GetEntry(data->iconNameIndex) : nullptr;
			const auto icon = iconEntry ? std::wstring(iconEntry->value) : L"";
			const auto description = data && data->description ? std::wstring(data->description) : L"";

			return {
				itemInfo.objectId,
				itemInfo.itemId,
				itemInfo.mana,
				name,
				icon,
				description,
				(uint16_t)(data ? data->weight : 0)
			};
		}

		const EtcData GetEtcData(const DTO::ItemData& itemInfo) const
		{
			const auto& baseData = GetBaseData(itemInfo);

			return {
				baseData.objectId,
				baseData.itemId,
				baseData.mana,
				baseData.name,
				baseData.iconName,
				baseData.description,
				baseData.weight,
				itemInfo.amount,
				itemInfo.isQuest
			};
		}

		const ArmorData GetArmorData(const DTO::ItemData& itemInfo) const
		{
			const auto& baseData = GetBaseData(itemInfo);

			const auto casted = static_cast<const FL2ArmorItemData*>(GetItemData(itemInfo.itemId));

			const auto setEffect = casted && casted->setEffect ? std::wstring(casted->setEffect) : L"";
			const auto addSetEffect = casted && casted->setEffect ? std::wstring(casted->setEffect) : L"";
			const auto enchantEffect = casted && casted->enchantEffect ? std::wstring(casted->enchantEffect) : L"";

			return {
				baseData.objectId,
				baseData.itemId,
				baseData.mana,
				baseData.name,
				baseData.iconName,
				baseData.description,
				baseData.weight,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				casted ? static_cast<Enums::ArmorTypeEnum>(casted->armorType) : Enums::ArmorTypeEnum::none,
				casted ? static_cast<Enums::CrystalTypeEnum>(casted->crystalType) : Enums::CrystalTypeEnum::none,
				m_EnchantHelper.GetDefenseEnchantValue(casted ? casted->pDefense : 0, itemInfo.enchantLevel),
				m_EnchantHelper.GetDefenseEnchantValue(casted ? casted->mDefense : 0, itemInfo.enchantLevel),
				setEffect,
				addSetEffect,
				enchantEffect
			};
		}

		const ShieldData GetShieldData(const DTO::ItemData& itemInfo, const FL2WeaponItemData* itemData) const
		{
			const auto& baseData = GetBaseData(itemInfo);

			return {
				baseData.objectId,
				baseData.itemId,
				baseData.mana,
				baseData.name,
				baseData.iconName,
				baseData.description,
				baseData.weight,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				itemData ? static_cast<Enums::CrystalTypeEnum>(itemData->crystalType) : Enums::CrystalTypeEnum::none,
				(int16_t)(itemData ? itemData->shieldEvasion : 0),
				m_EnchantHelper.GetDefenseEnchantValue(itemData ? itemData->shieldPdef : 0, itemInfo.enchantLevel),
				(uint16_t)(itemData ? itemData->shieldDefRate : 0)
			};
		}

		const WeaponData GetWeaponData(const DTO::ItemData& itemInfo, const FL2WeaponItemData* itemData) const
		{
			const auto& baseData = GetBaseData(itemInfo);

			return {
				baseData.objectId,
				baseData.itemId,
				baseData.mana,
				baseData.name,
				baseData.iconName,
				baseData.description,
				baseData.weight,
				itemInfo.isEquipped > 0,
				itemInfo.enchantLevel,
				itemData ? static_cast<Enums::WeaponTypeEnum>(itemData->weaponType) : Enums::WeaponTypeEnum::none,
				itemData ? static_cast<Enums::CrystalTypeEnum>(itemData->crystalType) : Enums::CrystalTypeEnum::none,
				(uint8_t)(itemData ? itemData->rndDamage : 0),
				m_EnchantHelper.GetPAttackEnchantValue(itemData->weaponType, itemInfo.isTwoHanded, itemData->crystalType, itemData ? itemData->pAttack : 0, itemInfo.enchantLevel),
				m_EnchantHelper.GetMAttackEnchantValue(itemData->crystalType, itemData ? itemData->mAttack : 0, itemInfo.enchantLevel),
				(uint16_t)(itemData ? itemData->critical : 0),
				(int8_t)(itemData ? itemData->hitModify : 0),
				(uint16_t)(itemData ? itemData->atkSpd : 0),
				(uint8_t)(itemData ? itemData->mpConsume : 0),
				(uint8_t)(itemData ? itemData->soulshotCount : 0),
				(uint8_t)(itemData ? itemData->spiritshotCount : 0)
			};
		}

		const FL2ItemDataBase* GetItemData(const uint32_t itemId) const
		{
			return m_L2GameData.GetItemData(itemId);
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
		const EnchantHelper& m_EnchantHelper;
	};
}