#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class CreatureDiedEvent : public Event
	{
	public:
		static constexpr const char* name = "creatureDied";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const uint32_t GetCreatureId() const
		{
			return m_CreatureId;
		}

		const std::vector<int32_t>& GetCreatureInfo() const
		{
			return m_CreatureInfo;
		}

		CreatureDiedEvent(uint32_t creatureId, const std::vector<int32_t>& creatureInfo) :
			m_CreatureId(creatureId),
			m_CreatureInfo(creatureInfo)
		{

		}

		CreatureDiedEvent() = delete;
		virtual ~CreatureDiedEvent() = default;

	private:
		const uint32_t m_CreatureId;
		const std::vector<int32_t> m_CreatureInfo;
	};
}