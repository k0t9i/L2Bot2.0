#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class CrystalType : int8_t
	{
		none = -1,
		ng,
		d,
		c,
		b,
		a,
		s
	};
}