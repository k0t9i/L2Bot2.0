#pragma once
#include <cstdint>
#include <functional>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class VitalStats : public Serializers::Serializable, public Entities::Hashable
	{
	public:
		const bool IsDead() const
		{
			return m_IsDead || (m_MaxHp > 0 && m_Hp <= 0);
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
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint32_t>{}(m_MaxHp),
				std::hash<uint32_t>{}(m_Hp),
				std::hash<uint32_t>{}(m_MaxMp),
				std::hash<uint32_t>{}(m_Mp),
				std::hash<uint32_t>{}(m_MaxCp),
				std::hash<uint32_t>{}(m_Cp),
				std::hash<bool>{}(m_IsDead)
			});
		}
		void LoadFromOther(VitalStats other)
		{
			m_MaxHp = other.m_MaxHp;
			m_Hp = other.m_Hp;
			m_MaxMp = other.m_MaxMp;
			m_Mp = other.m_Mp;
			m_MaxCp = other.m_MaxCp;
			m_Cp = other.m_Cp;
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
				{ L"cp", std::to_wstring(m_Cp) },
				{ L"isDead", std::to_wstring(IsDead()) }
			};
		}
		void MarkAsDead()
		{
			m_IsDead = true;;
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
		uint32_t m_IsDead = false;
	};
}
