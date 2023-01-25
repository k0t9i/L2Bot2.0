#pragma once

#include <cstdint>
#include <map>
#include "../GameStructs/GameStructs.h"

namespace Interlude
{
	class EnchantHelper
	{
	public:
		EnchantHelper()
		{
			m_PAttackDeltas =
			{
				{
					L2::CrystalType::D,
					{
						{ WeaponType::common, 2 },
						{ WeaponType::twoHand, 2 },
						{ WeaponType::bow, 4 },
					}
				},
				{
					L2::CrystalType::C,
					{
						{ WeaponType::common, 3 },
						{ WeaponType::twoHand, 4 },
						{ WeaponType::bow, 6 },
					}
				},
				{
					L2::CrystalType::B,
					{
						{ WeaponType::common, 3 },
						{ WeaponType::twoHand, 4 },
						{ WeaponType::bow, 6 },
					}
				},
				{
					L2::CrystalType::A,
					{
						{ WeaponType::common, 4 },
						{ WeaponType::twoHand, 5 },
						{ WeaponType::bow, 8 },
					}
				},
				{
					L2::CrystalType::S,
					{
						{ WeaponType::common, 5 },
						{ WeaponType::twoHand, 6 },
						{ WeaponType::bow, 10 },
					}
				},
			};

			m_MAttackDeltas =
			{
				{ L2::CrystalType::D, 2 },
				{ L2::CrystalType::C, 3 },
				{ L2::CrystalType::B, 3 },
				{ L2::CrystalType::A, 3 },
				{ L2::CrystalType::S, 4 },
			};
		}
		virtual ~EnchantHelper() = default;

		const std::uint32_t GetPAttackEnchantValue(const L2::WeaponType weaponType, const bool is2hand, const L2::CrystalType crystalType, const std::uint16_t baseValue, const std::uint16_t enchantLevel) const
		{
			if (m_PAttackDeltas.find(crystalType) == m_PAttackDeltas.end())
			{
				return baseValue;
			}

			WeaponType type = WeaponType::common;
			if (weaponType == L2::WeaponType::BOW)
			{
				type = WeaponType::bow;
			}
			else if (is2hand)
			{
				type = WeaponType::twoHand;
			}

			auto enchantDelta = m_PAttackDeltas.at(crystalType).at(type);

			return GetEnchantValue(baseValue, enchantLevel, enchantDelta, enchantDelta * 2);
		}

		const std::uint32_t GetMAttackEnchantValue(const L2::CrystalType crystalType, const std::uint16_t baseValue, const std::uint16_t enchantLevel) const
		{
			if (m_MAttackDeltas.find(crystalType) == m_MAttackDeltas.end())
			{
				return baseValue;
			}

			auto enchantDelta = m_MAttackDeltas.at(crystalType);

			return GetEnchantValue(baseValue, enchantLevel, enchantDelta, enchantDelta * 2);
		}

		const std::uint32_t GetDefenseEnchantValue(const std::uint16_t baseValue, const std::uint16_t enchantLevel) const
		{
			return GetEnchantValue(baseValue, enchantLevel, 1, 3);
		}

	private:
		const std::uint32_t GetEnchantValue(const std::uint16_t baseValue, const std::uint16_t enchantLevel, const uint8_t enchantDelta, const uint8_t overenchantDelta) const
		{
			if (baseValue == 0)
			{
				return 0;
			}

			return baseValue + enchantLevel * enchantDelta + max(enchantLevel - 3, 0) * (overenchantDelta - enchantDelta);
		}

	private:
		enum class WeaponType : uint8_t
		{
			common,
			twoHand,
			bow
		};

		std::map<L2::CrystalType, std::map<WeaponType, uint8_t>> m_PAttackDeltas;
		std::map<L2::CrystalType, uint8_t> m_MAttackDeltas;
	};
}