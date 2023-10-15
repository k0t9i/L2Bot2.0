#pragma once
#include <cstdint>
#include <functional>
#include "../Serializers/Serializable.h"
#include "../Entities/Hashable.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::ValueObjects
{
	class PermanentStats : public Serializers::Serializable, public Entities::Hashable
	{
	public:
		const uint16_t GetStr() const
		{
			return m_Str;
		}
		const uint16_t GetDex() const
		{
			return m_Dex;
		}
		const uint16_t GetCon() const
		{
			return m_Con;
		}
		const uint16_t GetInt() const
		{
			return m_Int;
		}
		const uint16_t GetMen() const
		{
			return m_Men;
		}
		const uint16_t GetWit() const
		{
			return m_Wit;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint16_t>{}(m_Str),
				std::hash<uint16_t>{}(m_Dex),
				std::hash<uint16_t>{}(m_Con),
				std::hash<uint16_t>{}(m_Int),
				std::hash<uint16_t>{}(m_Men),
				std::hash<uint16_t>{}(m_Wit)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"str", std::to_wstring(m_Str) },
				{ L"dex", std::to_wstring(m_Dex) },
				{ L"con", std::to_wstring(m_Con) },
				{ L"int", std::to_wstring(m_Int) },
				{ L"men", std::to_wstring(m_Men) },
				{ L"wit", std::to_wstring(m_Wit) }
			};
		}

		PermanentStats(
			uint16_t str,
			uint16_t dex,
			uint16_t con,
			uint16_t int_,
			uint16_t men,
			uint16_t wit
		) :
			m_Str(str),
			m_Dex(dex),
			m_Con(con),
			m_Int(int_),
			m_Men(men),
			m_Wit(wit)
		{
		}

		PermanentStats() = default;
		virtual ~PermanentStats() = default;

	private:
		uint16_t m_Str = 0;
		uint16_t m_Dex = 0;
		uint16_t m_Con = 0;
		uint16_t m_Int = 0;
		uint16_t m_Men = 0;
		uint16_t m_Wit = 0;
	};
}
