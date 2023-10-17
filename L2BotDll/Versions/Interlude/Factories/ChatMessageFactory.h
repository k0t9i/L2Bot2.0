#pragma once

#include "Domain/Entities/ChatMessage.h"
#include "Domain/DTO/ChatMessageData.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ChatMessageFactory
	{
	public:
		ChatMessageFactory() = default;
		virtual ~ChatMessageFactory() = default;

		std::shared_ptr<Entities::ChatMessage> Create(const DTO::ChatMessageData& message) const
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