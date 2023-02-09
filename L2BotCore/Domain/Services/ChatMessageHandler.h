#pragma once

#include <vector>
#include "../Repositories/ChatMessageRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class ChatMessageHandler
	{
	public:
		ChatMessageHandler(Repositories::ChatMessageRepositoryInterface& repository) : m_Repository(repository)
		{

		}
		ChatMessageHandler() = delete;
		virtual ~ChatMessageHandler() = default;

		virtual const std::vector<ValueObjects::ChatMessage> GetMessages()
		{
			return m_Repository.GetMessages();
		}
	private:
		Repositories::ChatMessageRepositoryInterface& m_Repository;
	};
}
