#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

class SkillCreatedEvent : public Event
{
public:
	static constexpr const char* name = "skillCreated";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const std::vector<int32_t>& GetSkillInfo() const
	{
		return m_SkillInfo;
	}

	SkillCreatedEvent(const std::vector<int32_t> skillInfo) :
		m_SkillInfo(skillInfo)
	{

	}

	SkillCreatedEvent() = delete;
	virtual ~SkillCreatedEvent() = default;

private:
	const std::vector<int32_t> m_SkillInfo;
};