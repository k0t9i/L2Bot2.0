#pragma once
#include <cstdint>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class Reputation : public Serializers::Serializable
	{
	public:
		const bool IsPlayerKiller() const
		{
			return m_Karma > 0;
		}
		const uint16_t GetKarma() const
		{
			return m_Karma;
		}
		const uint16_t GetPkKills() const
		{
			return m_PkKills;
		}
		const uint16_t GetPvpKills() const
		{
			return m_PvpKills;
		}
		const uint8_t GetRecRemaining() const
		{
			return m_RecRemaining;
		}
		const uint8_t GetEvalScore() const
		{
			return m_EvalScore;
		}
		const bool IsEqual(const Reputation* other) const
		{
			return m_Karma == other->m_Karma &&
				m_PkKills == other->m_PkKills &&
				m_PvpKills == other->m_PvpKills &&
				m_RecRemaining == other->m_RecRemaining &&
				m_EvalScore == other->m_EvalScore;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "karma", std::to_string(m_Karma) },
				{ "pkKills", std::to_string(m_PkKills) },
				{ "pvpKills", std::to_string(m_PvpKills) },
				{ "recRemaining", std::to_string(m_RecRemaining) },
				{ "evalScore", std::to_string(m_EvalScore) }
			};
		}

		Reputation(
			uint16_t karma,
			uint16_t pkKills,
			uint16_t pvpKills,
			uint8_t recRemaining,
			uint8_t evalScore
		) :
			m_Karma(karma),
			m_PkKills(pkKills),
			m_PvpKills(pvpKills),
			m_RecRemaining(recRemaining),
			m_EvalScore(evalScore)
		{
		}

		Reputation() = default;
		virtual ~Reputation() = default;

	private:
		uint16_t m_Karma = 0;
		uint16_t m_PkKills = 0;
		uint16_t m_PvpKills = 0;
		uint8_t m_RecRemaining = 0;
		uint8_t m_EvalScore = 0;
	};
}
