#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class AbnormalEffectChangedEvent : public Event
	{
	public:
		static constexpr const char* name = "abnormalEffectChanged";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const std::vector<int32_t>& GetSkillInfo() const
		{
			return m_SkillInfo;
		}

		AbnormalEffectChangedEvent(const std::vector<int32_t> skillInfo) :
			m_SkillInfo(skillInfo)
		{

		}

		AbnormalEffectChangedEvent() = delete;
		virtual ~AbnormalEffectChangedEvent() = default;

	private:
		const std::vector<int32_t> m_SkillInfo;
	};
}