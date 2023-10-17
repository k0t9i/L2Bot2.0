#pragma once

#include <cstdint>

namespace L2Bot::Domain::Enums
{
	enum class ChatChannelEnum : uint8_t
	{
		all,
		shout,
		tell,
		party,
		clan,
		gm,
		petitionPlayer,
		petitionGm,
		trade,
		alliance,
		announcement,
		partyroomCommander = 15,
		partyroomAll,
		heroVoice,
		log = 255
	};
}