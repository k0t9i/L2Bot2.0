#pragma once

#include <vector>
#include "../Repositories/ChatMessageRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class ChatMessageService
	{
	public:
		ChatMessageService(Repositories::ChatMessageRepositoryInterface& repository) : m_Repository(repository)
		{

		}
		ChatMessageService() = delete;
		virtual ~ChatMessageService() = default;

		virtual const std::vector<ValueObjects::ChatMessage> GetMessages()
		{
			return m_Repository.GetMessages();
		}
	private:
		Repositories::ChatMessageRepositoryInterface& m_Repository;
	};
}
