#pragma once

#include "Event.h"

class HeroDeletedEvent : public Event
{
public:
	static constexpr const char* name = "heroDeleted";

	const std::string GetName() const
	{
		return std::string(name);
	}

	HeroDeletedEvent() = default;
	virtual ~HeroDeletedEvent() = default;
};