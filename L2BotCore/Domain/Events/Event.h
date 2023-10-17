#pragma once

#include <string>

namespace L2Bot::Domain::Events
{
	class Event
	{
	public:
		virtual const std::string GetName() const = 0;

		Event() = default;
		virtual ~Event() = default;
	};
}