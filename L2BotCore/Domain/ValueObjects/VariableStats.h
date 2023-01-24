#pragma once
#include <cstdint>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class VariableStats : public Serializers::Serializable
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
		const bool IsEqual(const VariableStats* other) const
		{
			return m_Accuracy == other->m_Accuracy &&
				m_CritRate == other->m_CritRate &&
				m_PAttack == other->m_PAttack &&
				m_AttackSpeed == other->m_AttackSpeed &&
				m_PDefense == other->m_PDefense &&
				m_Evasion == other->m_Evasion &&
				m_MAttack == other->m_MAttack &&
				m_MDefense == other->m_MDefense &&
				m_CastingSpeed == other->m_CastingSpeed;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "accuracy", std::to_string(m_Accuracy) },
				{ "critRate", std::to_string(m_CritRate) },
				{ "pAttack", std::to_string(m_PAttack) },
				{ "attackSpeed", std::to_string(m_AttackSpeed) },
				{ "pDefense", std::to_string(m_PDefense) },
				{ "evasion", std::to_string(m_Evasion) },
				{ "mAttack", std::to_string(m_MAttack) },
				{ "mDefense", std::to_string(m_MDefense) },
				{ "castingSpeed", std::to_string(m_CastingSpeed) }
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
