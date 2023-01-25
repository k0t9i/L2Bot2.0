#pragma once

#include "Domain/ValueObjects/ChatMessage.h"
#include "../../../DTO/ChatMessage.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ChatMessageFactory
	{
	public:
		ChatMessageFactory() = default;
		virtual ~ChatMessageFactory() = default;

		ValueObjects::ChatMessage Create(const ChatMessage& message) const
		{
			return ValueObjects::ChatMessage{
				message.objectId,
				static_cast<Enums::ChatChannelEnum>(message.channel),
				message.name,
				message.text
			};
		}
	};
}