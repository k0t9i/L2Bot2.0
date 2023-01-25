#pragma once

#include <vector>
#include <shared_mutex>
#include "Domain/Repositories/ChatMessageRepositoryInterface.h"
#include "../Factories/ChatMessageFactory.h"
#include "../../../Events/ChatMessageCreatedEvent.h"
#include "../../../Events/EventDispatcher.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ChatMessageRepository : public Repositories::ChatMessageRepositoryInterface
	{
	public:
		const std::vector<ValueObjects::ChatMessage> GetMessages() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			const auto result = m_Messages;
			m_Messages.clear();

			return result;
		}

		ChatMessageRepository(const ChatMessageFactory& factory) :
			m_Factory(factory)
		{
			EventDispatcher::GetInstance().Subscribe(ChatMessageCreatedEvent::name, [this](const Event& evt) {
				OnMessageCreated(evt);
			});
		}

		void OnMessageCreated(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ChatMessageCreatedEvent::name)
			{
				const auto casted = static_cast<const ChatMessageCreatedEvent&>(evt);

				m_Messages.push_back(m_Factory.Create(casted.GetChatMessage()));
			}
		}

		ChatMessageRepository() = delete;
		virtual ~ChatMessageRepository() = default;

	private:
		const ChatMessageFactory& m_Factory;
		std::vector<ValueObjects::ChatMessage> m_Messages;
		std::shared_timed_mutex m_Mutex;
	};
}