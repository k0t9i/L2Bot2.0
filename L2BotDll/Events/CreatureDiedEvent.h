#pragma once

#include <cstdint>
#include "Event.h"

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

	CreatureDiedEvent(uint32_t creatureId) :
		m_CreatureId(creatureId)
	{

	}

	CreatureDiedEvent() = delete;
	virtual ~CreatureDiedEvent() = default;

private:
	const uint32_t m_CreatureId;
};