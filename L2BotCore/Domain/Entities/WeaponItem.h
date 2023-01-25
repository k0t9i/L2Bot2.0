#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"
#include "../Enums/WeaponTypeEnum.h"
#include "../Enums/CrystalTypeEnum.h"

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
			m_PAttack = casted->m_PAttack;
			m_MAttack = casted->m_MAttack;
			m_RndDamage = casted->m_RndDamage;
			m_Critical = casted->m_Critical;
			m_HitModify = casted->m_HitModify;
			m_AttackSpeed = casted->m_AttackSpeed;
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
				m_PAttack,
				m_MAttack,
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
				m_PAttack == casted->m_PAttack &&
				m_MAttack == casted->m_MAttack &&
				m_RndDamage == casted->m_RndDamage &&
				m_Critical == casted->m_Critical &&
				m_HitModify == casted->m_HitModify &&
				m_AttackSpeed == casted->m_AttackSpeed &&
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
				result.push_back({ "attackSpeed", std::to_string(m_AttackSpeed) });
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
			if (m_PrevState.isNewState || m_PAttack != m_PrevState.pAttack)
			{
				result.push_back({ "pAttack", std::to_string(m_PAttack) });
			}
			if (m_PrevState.isNewState || m_MAttack != m_PrevState.mAttack)
			{
				result.push_back({ "mAttack", std::to_string(m_MAttack) });
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
			const Enums::WeaponTypeEnum weaponType,
			const Enums::CrystalTypeEnum crystalType,
			const uint8_t rndDamage,
			const uint32_t pAttack,
			const uint32_t mAttack,
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
				Enums::ItemTypeEnum::weapon
			),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_WeaponType(weaponType),
			m_CrystalType(crystalType),
			m_RndDamage(rndDamage),
			m_PAttack(pAttack),
			m_MAttack(mAttack),
			m_Critical(critical),
			m_HitModify(hitModify),
			m_AttackSpeed(atkSpd),
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
			m_PAttack(other->m_PAttack),
			m_MAttack(other->m_MAttack),
			m_Critical(other->m_Critical),
			m_HitModify(other->m_HitModify),
			m_AttackSpeed(other->m_AttackSpeed),
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
			uint32_t pAttack = 0;
			uint32_t mAttack = 0;

			bool isNewState = true;
		};

	private:
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		Enums::WeaponTypeEnum m_WeaponType = Enums::WeaponTypeEnum::none;
		Enums::CrystalTypeEnum m_CrystalType = Enums::CrystalTypeEnum::none;
		uint8_t m_RndDamage = 0;
		uint32_t m_PAttack = 0;
		uint32_t m_MAttack = 0;
		uint16_t m_Critical = 0;
		int8_t m_HitModify = 0;
		uint16_t m_AttackSpeed = 0;
		uint8_t m_MpConsume = 0;
		uint8_t m_SoulshotCount = 0;
		uint8_t m_SpiritshotCount = 0;

		GetState m_PrevState = GetState();
	};
}
