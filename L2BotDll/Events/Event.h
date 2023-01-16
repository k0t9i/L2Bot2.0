#pragma once

#include <string>

class Event
{
public:
	virtual const std::string GetName() const = 0;

	Event() = default;
	virtual ~Event() = default;
};