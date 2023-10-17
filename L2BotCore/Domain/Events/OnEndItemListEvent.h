#pragma once

#include "Event.h"

namespace L2Bot::Domain::Events
{
	class OnEndItemListEvent : public Event
	{
	public:
		static constexpr const char* name = "onEndItemList";

		const std::string GetName() const
		{
			return std::string(name);
		}

		OnEndItemListEvent() = default;
		virtual ~OnEndItemListEvent() = default;
	};
}