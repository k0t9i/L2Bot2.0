#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class SkillUsedEvent : public Event
	{
	public:
		static constexpr const char* name = "skillUsed";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const std::vector<int32_t>& GetSkillInfo() const
		{
			return m_SkillInfo;
		}

		SkillUsedEvent(const std::vector<int32_t> skillInfo) :
			m_SkillInfo(skillInfo)
		{

		}

		SkillUsedEvent() = delete;
		virtual ~SkillUsedEvent() = default;

	private:
		const std::vector<int32_t> m_SkillInfo;
	};
}