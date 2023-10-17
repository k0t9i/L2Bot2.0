#pragma once

#include <vector>
#include <shared_mutex>
#include "../Factories/ChatMessageFactory.h"
#include "Domain/Events/ChatMessageCreatedEvent.h"
#include "Domain/Services/ServiceLocator.h"

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

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_Messages.clear();
		}

		void Init() override
		{
			Services::ServiceLocator::GetInstance().GetEventDispatcher()->Subscribe(Events::ChatMessageCreatedEvent::name, [this](const Events::Event& evt) {
				OnMessageCreated(evt);
			});
		}

		ChatMessageRepository(const ChatMessageFactory& factory) :
			m_Factory(factory)
		{
		}

		void OnMessageCreated(const Events::Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == Events::ChatMessageCreatedEvent::name)
			{
				const auto casted = static_cast<const Events::ChatMessageCreatedEvent&>(evt);

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