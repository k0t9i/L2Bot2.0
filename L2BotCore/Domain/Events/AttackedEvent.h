#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class AttackedEvent : public Event
	{
	public:
		static constexpr const char* name = "attacked";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const uint32_t GetAttackerId() const
		{
			return m_AttackerId;
		}

		const uint32_t GetTargetId() const
		{
			return m_TargetId;
		}

		AttackedEvent(const uint32_t attackerId, const uint32_t targetId) :
			m_AttackerId(attackerId),
			m_TargetId(targetId)
		{

		}

		AttackedEvent() = delete;
		virtual ~AttackedEvent() = default;

	private:
		const uint32_t m_AttackerId;
		const uint32_t m_TargetId;
	};
}