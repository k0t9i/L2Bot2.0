#pragma once
#include <cstdint>
#include "../ValueObjects/Transform.h"

namespace L2Bot::Domain::DTO
{
	struct WorldObject
	{
	public:
		const uint32_t id = 0;
		const ValueObjects::Transform transform = ValueObjects::Transform();
	};
}
