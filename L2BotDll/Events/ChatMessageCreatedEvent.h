#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ChatMessage.h"

class ChatMessageCreatedEvent : public Event
{
public:
	static constexpr const char* name = "chatMessageCreated";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const ChatMessage& GetChatMessage() const
	{
		return m_ChatMessage;
	}

	ChatMessageCreatedEvent(const ChatMessage chatMessage) :
		m_ChatMessage(chatMessage)
	{

	}

	ChatMessageCreatedEvent() = delete;
	virtual ~ChatMessageCreatedEvent() = default;

private:
	const ChatMessage m_ChatMessage;
};