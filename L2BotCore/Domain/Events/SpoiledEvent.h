#pragma once

#include <cstdint>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class SpoiledEvent : public Event
	{
	public:
		static constexpr const char* name = "spoiled";

		const std::string GetName() const
		{
			return std::string(name);
		}

		SpoiledEvent() = default;
		virtual ~SpoiledEvent() = default;
	};
}