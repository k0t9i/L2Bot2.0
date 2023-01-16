#pragma once

#include <cstdint>
#include "Event.h"

class SkillCancelledEvent : public Event
{
public:
	static constexpr const char* name = "skillCancelled";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const uint32_t GetInitiatorId() const
	{
		return m_InitiatorId;
	}

	SkillCancelledEvent(const uint32_t initiatorId) :
		m_InitiatorId(initiatorId)
	{

	}

	SkillCancelledEvent() = delete;
	virtual ~SkillCancelledEvent() = default;

private:
	const uint32_t m_InitiatorId;
};