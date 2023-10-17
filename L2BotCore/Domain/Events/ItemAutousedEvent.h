#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class ItemAutousedEvent : public Event
	{
	public:
		static constexpr const char* name = "itemAutoused";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const std::vector<uint32_t>& GetAutouseInfo() const
		{
			return m_AutouseInfo;
		}

		ItemAutousedEvent(const std::vector<uint32_t> autouseInfo) :
			m_AutouseInfo(autouseInfo)
		{

		}

		ItemAutousedEvent() = delete;
		virtual ~ItemAutousedEvent() = default;

	private:
		const std::vector<uint32_t> m_AutouseInfo;
	};
}