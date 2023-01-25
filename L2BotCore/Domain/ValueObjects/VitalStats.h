#pragma once
#include <cstdint>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class VitalStats : public Serializers::Serializable
	{
	public:
		const bool IsAlive() const
		{
			return m_MaxHp <= 0 || m_Hp > 0;
		}
		const uint32_t GetMaxHp() const
		{
			return m_MaxHp;
		}
		const uint32_t GetHp() const
		{
			return m_Hp;
		}
		const uint32_t GetMaxMp() const
		{
			return m_MaxMp;
		}
		const uint32_t GetMp() const
		{
			return m_Mp;
		}
		const uint32_t GetMaxCp() const
		{
			return m_MaxCp;
		}
		const uint32_t GetCp() const
		{
			return m_Cp;
		}
		const bool IsEqual(const VitalStats* other) const
		{
			return m_MaxHp == other->m_MaxHp &&
				m_Hp == other->m_Hp &&
				m_MaxMp == other->m_MaxMp &&
				m_Mp == other->m_Mp &&
				m_MaxCp == other->m_MaxCp &&
				m_Cp == other->m_Cp;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"maxHp", std::to_wstring(m_MaxHp) },
				{ L"hp", std::to_wstring(m_Hp) },
				{ L"maxMp", std::to_wstring(m_MaxMp) },
				{ L"mp", std::to_wstring(m_Mp) },
				{ L"maxCp", std::to_wstring(m_MaxCp) },
				{ L"cp", std::to_wstring(m_Cp) }
			};
		}

		VitalStats(
			uint32_t maxHp,
			uint32_t hp,
			uint32_t maxMp,
			uint32_t mp,
			uint32_t maxCp,
			uint32_t cp
		) :
			m_MaxHp(maxHp),
			m_Hp(hp),
			m_MaxMp(maxMp),
			m_Mp(mp),
			m_MaxCp(maxCp),
			m_Cp(cp)
		{
		}

		VitalStats() = default;
		virtual ~VitalStats() = default;

	private:
		uint32_t m_MaxHp = 0;
		uint32_t m_Hp = 0;
		uint32_t m_MaxMp = 0;
		uint32_t m_Mp = 0;
		uint32_t m_MaxCp = 0;
		uint32_t m_Cp = 0;
	};
}
