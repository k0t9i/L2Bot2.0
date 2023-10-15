#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "BaseItem.h"
#include "../Enums/WeaponTypeEnum.h"
#include "../Enums/CrystalTypeEnum.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class WeaponItem : public BaseItem
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
			const Enums::WeaponTypeEnum weaponType,
			const Enums::CrystalTypeEnum crystalType,
			const uint8_t rndDamage,
			const uint32_t pAttack,
			const uint32_t mAttack,
			const uint16_t critical,
			const int8_t hitModify,
			const uint16_t attackSpeed,
			const uint8_t mpConsume,
			const uint8_t soulshotCount,
			const uint8_t spiritshotCount
		) {
			BaseItem::Update(itemId, mana, name, iconName, description, weight);

			m_IsEquipped = isEquipped;
			m_EnchantLevel = enchantLevel;
			m_WeaponType = weaponType;
			m_CrystalType = crystalType;
			m_RndDamage = rndDamage;
			m_PAttack = pAttack;
			m_MAttack = mAttack;
			m_Critical = critical;
			m_HitModify = hitModify;
			m_AttackSpeed = attackSpeed;
			m_MpConsume = mpConsume;
			m_SoulshotCount = soulshotCount;
			m_SpiritshotCount = spiritshotCount;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				BaseItem::GetHash(),
				std::hash<bool>{}(m_IsEquipped),
				std::hash<uint16_t>{}(m_EnchantLevel),
				std::hash<Enums::WeaponTypeEnum>{}(m_WeaponType),
				std::hash<Enums::CrystalTypeEnum>{}(m_CrystalType),
				std::hash<uint8_t>{}(m_RndDamage),
				std::hash<uint32_t>{}(m_PAttack),
				std::hash<uint32_t>{}(m_MAttack),
				std::hash<uint16_t>{}(m_Critical),
				std::hash<int8_t>{}(m_HitModify),
				std::hash<uint16_t>{}(m_AttackSpeed),
				std::hash<uint8_t>{}(m_MpConsume),
				std::hash<uint8_t>{}(m_SoulshotCount),
				std::hash<uint8_t>{}(m_SpiritshotCount)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			result.push_back({ L"weaponType", std::to_wstring(static_cast<uint8_t>(m_WeaponType)) });
			result.push_back({ L"crystalType", std::to_wstring(static_cast<int8_t>(m_CrystalType)) });
			result.push_back({ L"rndDamage", std::to_wstring(m_RndDamage) });
			result.push_back({ L"critical", std::to_wstring(m_Critical) });
			result.push_back({ L"hitModify", std::to_wstring(m_HitModify) });
			result.push_back({ L"attackSpeed", std::to_wstring(m_AttackSpeed) });
			result.push_back({ L"mpConsume", std::to_wstring(m_MpConsume) });
			result.push_back({ L"soulshotCount", std::to_wstring(m_SoulshotCount) });
			result.push_back({ L"spiritshotCount", std::to_wstring(m_SpiritshotCount) });
			result.push_back({ L"isEquipped", std::to_wstring(m_IsEquipped) });
			result.push_back({ L"enchantLevel", std::to_wstring(m_EnchantLevel) });
			result.push_back({ L"pAttack", std::to_wstring(m_PAttack) });
			result.push_back({ L"mAttack", std::to_wstring(m_MAttack) });

			return result;
		}

		WeaponItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
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
			const uint16_t attackSpeed,
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
			m_AttackSpeed(attackSpeed),
			m_MpConsume(mpConsume),
			m_SoulshotCount(soulshotCount),
			m_SpiritshotCount(spiritshotCount)
		{
		}

		WeaponItem() = default;
		virtual ~WeaponItem() = default;

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
	};
}
