#pragma once

#include <cstdint>

namespace L2Bot::Domain::Logger
{
	enum class LogLevel : uint8_t
	{
		error,
		warning,
		info,
		app
	};
}