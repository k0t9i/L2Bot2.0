#pragma once

#include <memory>
#include "../Events/EventDispatcher.h"
#include "../Logger/Logger.h"

namespace L2Bot::Domain::DTO
{
	struct UseSkillParams
	{
		uint32_t skillId;
		bool isForced;
		bool isShiftPressed;
	};
}