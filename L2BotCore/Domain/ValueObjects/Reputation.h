#pragma once
#include <cstdint>
#include <functional>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class Reputation : public Serializers::Serializable, public Entities::Hashable
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
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint16_t>{}(m_Karma),
				std::hash<uint16_t>{}(m_PkKills),
				std::hash<uint16_t>{}(m_PvpKills),
				std::hash<uint8_t>{}(m_RecRemaining),
				std::hash<uint8_t>{}(m_EvalScore)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"karma", std::to_wstring(m_Karma) },
				{ L"pkKills", std::to_wstring(m_PkKills) },
				{ L"pvpKills", std::to_wstring(m_PvpKills) },
				{ L"recRemaining", std::to_wstring(m_RecRemaining) },
				{ L"evalScore", std::to_wstring(m_EvalScore) }
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
