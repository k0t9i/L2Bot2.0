#pragma once
#include <cstdint>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class ExperienceInfo : public Serializers::Serializable
	{
	public:
		const uint8_t GetLevel() const
		{
			return m_Level;
		}
		const uint8_t GetExp() const
		{
			return m_Exp;
		}
		const uint8_t GetSp() const
		{
			return m_Sp;
		}
		const bool IsEqual(const ExperienceInfo* other) const
		{
			return m_Level == other->m_Level && m_Exp == other->m_Exp && m_Sp == other->m_Sp;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ "level", std::to_string(m_Level) },
				{ "exp", std::to_string(m_Exp) },
				{ "sp", std::to_string(m_Sp) }
			};
		}

		ExperienceInfo(
			const uint8_t level,
			const uint32_t exp,
			const uint32_t sp
		) :
			m_Level(level),
			m_Exp(exp),
			m_Sp(sp)
		{
		}

		ExperienceInfo() = default;
		virtual ~ExperienceInfo() = default;
	private:
		uint8_t m_Level = 0;
		uint32_t m_Exp = 0;
		uint32_t m_Sp = 0;
	};
}
