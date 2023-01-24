#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"
#include "../Enums/WeaponType.h"
#include "../Enums/CrystalType.h"

namespace L2Bot::Domain::Entities
{
	class WeaponItem : public BaseItem
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const WeaponItem* casted = static_cast<const WeaponItem*>(other);

			BaseItem::Update(other);

			m_IsEquipped = casted->m_IsEquipped;
			m_EnchantLevel = casted->m_EnchantLevel;
			m_WeaponType = casted->m_WeaponType;
			m_CrystalType = casted->m_CrystalType;
			m_PAtk = casted->m_PAtk;
			m_MAtk = casted->m_MAtk;
			m_RndDamage = casted->m_RndDamage;
			m_Critical = casted->m_Critical;
			m_HitModify = casted->m_HitModify;
			m_AtkSpd = casted->m_AtkSpd;
			m_MpConsume = casted->m_MpConsume;
			m_SoulshotCount = casted->m_SoulshotCount;
			m_SpiritshotCount = casted->m_SpiritshotCount;
		}
		void SaveState() override
		{
			BaseItem::SaveState();
			m_PrevState =
			{
				m_IsEquipped,
				m_EnchantLevel,
				m_PAtk,
				m_MAtk,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const WeaponItem* casted = static_cast<const WeaponItem*>(other);
			return BaseItem::IsEqual(other) &&
				m_IsEquipped == casted->m_IsEquipped &&
				m_EnchantLevel == casted->m_EnchantLevel &&
				m_WeaponType == casted->m_WeaponType &&
				m_CrystalType == casted->m_CrystalType &&
				m_PAtk == casted->m_PAtk &&
				m_MAtk == casted->m_MAtk &&
				m_RndDamage == casted->m_RndDamage &&
				m_Critical == casted->m_Critical &&
				m_HitModify == casted->m_HitModify &&
				m_AtkSpd == casted->m_AtkSpd &&
				m_MpConsume == casted->m_MpConsume &&
				m_SoulshotCount == casted->m_SoulshotCount &&
				m_SpiritshotCount == casted->m_SpiritshotCount;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			if (m_PrevState.isNewState)
			{
				result.push_back({ "weaponType", std::to_string(static_cast<uint8_t>(m_WeaponType)) });
				result.push_back({ "crystalType", std::to_string(static_cast<int8_t>(m_CrystalType)) });
				result.push_back({ "rndDamage", std::to_string(m_RndDamage) });
				result.push_back({ "critical", std::to_string(m_Critical) });
				result.push_back({ "hitModify", std::to_string(m_HitModify) });
				result.push_back({ "atkSpd", std::to_string(m_AtkSpd) });
				result.push_back({ "mpConsume", std::to_string(m_MpConsume) });
				result.push_back({ "soulshotCount", std::to_string(m_SoulshotCount) });
				result.push_back({ "spiritshotCount", std::to_string(m_SpiritshotCount) });
			}

			if (m_PrevState.isNewState || m_IsEquipped != m_PrevState.isEquipped)
			{
				result.push_back({ "isEquipped", std::to_string(m_IsEquipped) });
			}
			if (m_PrevState.isNewState || m_EnchantLevel != m_PrevState.enchantLevel)
			{
				result.push_back({ "enchantLevel", std::to_string(m_EnchantLevel) });
			}
			if (m_PrevState.isNewState || m_PAtk != m_PrevState.pAtk)
			{
				result.push_back({ "pAtk", std::to_string(m_PAtk) });
			}
			if (m_PrevState.isNewState || m_MAtk != m_PrevState.mAtk)
			{
				result.push_back({ "mAtk", std::to_string(m_MAtk) });
			}

			return result;
		}

		WeaponItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::string& name,
			const std::string& iconName,
			const std::string& description,
			const uint16_t weight,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const Enums::WeaponType weaponType,
			const Enums::CrystalType crystalType,
			const uint8_t rndDamage,
			const uint16_t pAtk,
			const uint16_t mAtk,
			const uint16_t critical,
			const int8_t hitModify,
			const uint16_t atkSpd,
			const uint8_t mpConsume,
			const uint8_t soulshotCount,
			const uint8_t spiritshotCount
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
				Enums::ItemType::weapon
			),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_WeaponType(weaponType),
			m_CrystalType(crystalType),
			m_RndDamage(rndDamage),
			m_PAtk(pAtk),
			m_MAtk(mAtk),
			m_Critical(critical),
			m_HitModify(hitModify),
			m_AtkSpd(atkSpd),
			m_MpConsume(mpConsume),
			m_SoulshotCount(soulshotCount),
			m_SpiritshotCount(spiritshotCount)
		{
		}

		WeaponItem(const WeaponItem* other) :
			BaseItem(other),
			m_IsEquipped(other->m_IsEquipped),
			m_EnchantLevel(other->m_EnchantLevel),
			m_WeaponType(other->m_WeaponType),
			m_CrystalType(other->m_CrystalType),
			m_RndDamage(other->m_RndDamage),
			m_PAtk(other->m_PAtk),
			m_MAtk(other->m_MAtk),
			m_Critical(other->m_Critical),
			m_HitModify(other->m_HitModify),
			m_AtkSpd(other->m_AtkSpd),
			m_MpConsume(other->m_MpConsume),
			m_SoulshotCount(other->m_SoulshotCount),
			m_SpiritshotCount(other->m_SpiritshotCount)

		{
		}

		WeaponItem() = default;
		virtual ~WeaponItem() = default;

	private:
		struct GetState
		{
			bool isEquipped = 0;
			uint16_t enchantLevel = 0;
			uint16_t pAtk = 0;
			uint16_t mAtk = 0;

			bool isNewState = true;
		};

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::WeaponType m_WeaponType = Enums::WeaponType::none;
		Enums::CrystalType m_CrystalType = Enums::CrystalType::none;
		uint8_t m_RndDamage = 0;
		uint16_t m_PAtk = 0;
		uint16_t m_MAtk = 0;
		uint16_t m_Critical = 0;
		int8_t m_HitModify = 0;
		uint16_t m_AtkSpd = 0;
		uint8_t m_MpConsume = 0;
		uint8_t m_SoulshotCount = 0;
		uint8_t m_SpiritshotCount = 0;

		GetState m_PrevState = GetState();
	};
}
