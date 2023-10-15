#pragma once
#include <functional>
#include <cstdint>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class VariableStats : public Serializers::Serializable, public Entities::Hashable
	{
	public:
		const uint16_t GetAccuracy() const
		{
			return m_Accuracy;
		}
		const uint16_t GetCritRate() const
		{
			return m_CritRate;
		}
		const uint32_t GetPAttack() const
		{
			return m_PAttack;
		}
		const uint16_t GetAttackSpeed() const
		{
			return m_AttackSpeed;
		}
		const uint32_t GetPDefense() const
		{
			return m_PDefense;
		}
		const uint16_t GetEvasion() const
		{
			return m_Evasion;
		}
		const uint32_t GetMAttack() const
		{
			return m_MAttack;
		}
		const uint32_t GetMDefense() const
		{
			return m_MDefense;
		}
		const uint16_t GetCastingSpeed() const
		{
			return m_CastingSpeed;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint16_t>{}(m_Accuracy),
				std::hash<uint16_t>{}(m_CritRate),
				std::hash<uint32_t>{}(m_PAttack),
				std::hash<uint16_t>{}(m_AttackSpeed),
				std::hash<uint32_t>{}(m_PDefense),
				std::hash<uint16_t>{}(m_Evasion),
				std::hash<uint32_t>{}(m_MAttack),
				std::hash<uint32_t>{}(m_MDefense),
				std::hash<uint16_t>{}(m_CastingSpeed)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"accuracy", std::to_wstring(m_Accuracy) },
				{ L"critRate", std::to_wstring(m_CritRate) },
				{ L"pAttack", std::to_wstring(m_PAttack) },
				{ L"attackSpeed", std::to_wstring(m_AttackSpeed) },
				{ L"pDefense", std::to_wstring(m_PDefense) },
				{ L"evasion", std::to_wstring(m_Evasion) },
				{ L"mAttack", std::to_wstring(m_MAttack) },
				{ L"mDefense", std::to_wstring(m_MDefense) },
				{ L"castingSpeed", std::to_wstring(m_CastingSpeed) }
			};
		}

		VariableStats(
			uint16_t accuracy,
			uint16_t critRate,
			uint16_t pAttack,
			uint16_t attackSpeed,
			uint16_t pDefense,
			uint16_t evasion,
			uint16_t mAttack,
			uint16_t mDefense,
			uint16_t castingSpeed
		) :
			m_Accuracy(accuracy),
			m_CritRate(critRate),
			m_PAttack(pAttack),
			m_AttackSpeed(attackSpeed),
			m_PDefense(pDefense),
			m_Evasion(evasion),
			m_MAttack(mAttack),
			m_MDefense(mDefense),
			m_CastingSpeed(castingSpeed)
		{
		}

		VariableStats() = default;
		virtual ~VariableStats() = default;

	private:
		uint16_t m_Accuracy = 0;
		uint16_t m_CritRate = 0;
		uint32_t m_PAttack = 0;
		uint16_t m_AttackSpeed = 0;
		uint32_t m_PDefense = 0;
		uint16_t m_Evasion = 0;
		uint32_t m_MAttack = 0;
		uint32_t m_MDefense = 0;
		uint16_t m_CastingSpeed = 0;
	};
}
