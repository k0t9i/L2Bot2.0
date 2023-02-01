#pragma once

#include "Event.h"

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