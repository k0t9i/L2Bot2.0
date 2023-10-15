#pragma once

#include "Domain/Entities/ChatMessage.h"
#include "../../../DTO/ChatMessage.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ChatMessageFactory
	{
	public:
		ChatMessageFactory() = default;
		virtual ~ChatMessageFactory() = default;

		std::shared_ptr<Entities::ChatMessage> Create(const ChatMessage& message) const
		{
			return std::make_shared<Entities::ChatMessage>(
				message.objectId,
				static_cast<Enums::ChatChannelEnum>(message.channel),
				message.name,
				message.text
			);
		}
	};
}