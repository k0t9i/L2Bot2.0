#pragma once

#include "Domain/Enums/ChatChannelEnum.h"
#include "Domain/Events/ChatMessageCreatedEvent.h"
#include "Domain/Logger/LogChannel.h"
#include "Domain/Services/ServiceLocator.h"

using namespace L2Bot::Domain;

class ChatLogChannel : public Logger::LogChannel
{
public:
	ChatLogChannel(const Enums::ChatChannelEnum chatChannel, const std::vector<Logger::LogLevel> levels) :
		Logger::LogChannel(levels),
		m_ChatChannel(chatChannel)
	{
	};
	virtual ~ChatLogChannel() = default;

protected:
	void DoSendToChannel(const std::wstring& logEntry) override
	{
		Services::ServiceLocator::GetInstance().GetEventDispatcher()->Dispatch(
			Events::ChatMessageCreatedEvent
			{
				DTO::ChatMessageData
				{
					0,
					static_cast<uint8_t>(m_ChatChannel),
					L"",
					GetCurrentDateTime() + logEntry
				}
			}
		);
	}

private:
	const Enums::ChatChannelEnum m_ChatChannel;
};