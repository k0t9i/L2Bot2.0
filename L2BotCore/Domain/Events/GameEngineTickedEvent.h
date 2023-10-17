#pragma once

#include <cstdint>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class GameEngineTickedEvent : public Event
	{
	public:
		static constexpr const char* name = "gameEngineTicked";

		const std::string GetName() const
		{
			return std::string(name);
		}

		GameEngineTickedEvent() = default;
		virtual ~GameEngineTickedEvent() = default;
	};
}