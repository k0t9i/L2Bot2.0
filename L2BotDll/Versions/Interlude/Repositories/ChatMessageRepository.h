#pragma once

#include <vector>
#include <shared_mutex>
#include "../Factories/ChatMessageFactory.h"
#include "../../../Events/ChatMessageCreatedEvent.h"
#include "../../../Events/EventDispatcher.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ChatMessageRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			return m_Messages;
		}

		void Reset()
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Messages.clear();
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

				const auto message = m_Factory.Create(casted.GetChatMessage());
				m_Messages[message->GetId()] = message;
			}
		}

		ChatMessageRepository() = delete;
		virtual ~ChatMessageRepository() = default;

	private:
		const ChatMessageFactory& m_Factory;
		std::unordered_map<uint32_t, std::shared_ptr<Entities::EntityInterface>> m_Messages;
		std::shared_timed_mutex m_Mutex;
	};
}