#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

class HeroCreatedEvent : public Event
{
public:
	static constexpr const char* name = "heroCreated";

	const std::string GetName() const
	{
		return std::string(name);
	}

	HeroCreatedEvent() = default;
	virtual ~HeroCreatedEvent() = default;
};