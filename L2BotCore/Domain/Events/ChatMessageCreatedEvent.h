#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ChatMessageData.h"

namespace L2Bot::Domain::Events
{
	class ChatMessageCreatedEvent : public Event
	{
	public:
		static constexpr const char* name = "chatMessageCreated";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const DTO::ChatMessageData& GetChatMessage() const
		{
			return m_ChatMessage;
		}

		ChatMessageCreatedEvent(const DTO::ChatMessageData chatMessage) :
			m_ChatMessage(chatMessage)
		{

		}

		ChatMessageCreatedEvent() = delete;
		virtual ~ChatMessageCreatedEvent() = default;

	private:
		const DTO::ChatMessageData m_ChatMessage;
	};
}