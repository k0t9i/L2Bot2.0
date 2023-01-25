#pragma once
#include <vector>
#include "../ValueObjects/ChatMessage.h"

namespace L2Bot::Domain::Repositories
{
	class ChatMessageRepositoryInterface
	{
	public:
		virtual const std::vector<ValueObjects::ChatMessage> GetMessages() = 0;
	};
}
