#pragma once

#include <unordered_map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/ItemFactory.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Events/ItemCreatedEvent.h"
#include "../../../Events/ItemUpdatedEvent.h"
#include "../../../Events/ItemDeletedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../../../Events/ItemAutousedEvent.h"
#include "../../../Events/OnEndItemListEvent.h"
#include "../../../Events/EventDispatcher.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ItemRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			std::unordered_map<std::uint32_t, std::shared_ptr<Entities::EntityInterface>> result(m_Items.begin(), m_Items.end());
			return result;
		}

		const std::shared_ptr<Entities::BaseItem> GetItem(uint32_t objectId) const
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			if (m_Items.find(objectId) != m_Items.end())
			{
				return m_Items.at(objectId);
			}
			return nullptr;
		}

		ItemRepository(const NetworkHandlerWrapper& networkHandler, const ItemFactory& factory) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory)
		{
			EventDispatcher::GetInstance().Subscribe(ItemCreatedEvent::name, [this](const Event& evt) {
				OnItemCreated(evt);
			});
			EventDispatcher::GetInstance().Subscribe(ItemUpdatedEvent::name, [this](const Event& evt) {
				OnItemUpdated(evt);
			});
			EventDispatcher::GetInstance().Subscribe(ItemDeletedEvent::name, [this](const Event& evt) {
				OnItemDeleted(evt);
			});
			EventDispatcher::GetInstance().Subscribe(HeroDeletedEvent::name, [this](const Event& evt) {
				OnHeroDeleted(evt);
			});
			EventDispatcher::GetInstance().Subscribe(ItemAutousedEvent::name, [this](const Event& evt) {
				OnItemAutoused(evt);
			});
			EventDispatcher::GetInstance().Subscribe(OnEndItemListEvent::name, [this](const Event& evt) {
				OnEndItemList(evt);
			});
		}

		void OnEndItemList(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == OnEndItemListEvent::name)
			{
				for (auto it = m_Items.begin(); it != m_Items.end();)
				{
					if (m_NewItems.find(it->first) == m_NewItems.end())
					{
						it = m_Items.erase(it);
					}
					else
					{
						++it;
					}
				}

				m_IsNewCycle = true;
			}
		}
		
		void OnHeroDeleted(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == HeroDeletedEvent::name)
			{
				Reset();
			}
		}
		
		void OnItemAutoused(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ItemAutousedEvent::name)
			{
				const auto casted = static_cast<const ItemAutousedEvent&>(evt);
				const auto& data = casted.GetAutouseInfo();

				const auto itemId = data[0];
				const bool isEnabled = data[1] > 0;

				for (const auto& kvp : m_Items)
				{
					if (kvp.second->GetItemId() == itemId)
					{
						auto casted = std::dynamic_pointer_cast<Entities::EtcItem>(kvp.second);
						if (isEnabled) {
							casted->StartAutouse();
						}
						else
						{
							casted->StopAutouse();
						}
					}
				}
			}
		}

		void OnItemCreated(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ItemCreatedEvent::name)
			{
				if (m_IsNewCycle)
				{
					m_IsNewCycle = false;
					m_NewItems.clear();
				}

				const auto casted = static_cast<const ItemCreatedEvent&>(evt);
				const auto& data = casted.GetItemData();

				
				if (m_Items.find(data.objectId) == m_Items.end())
				{
					auto item = m_Factory.Create(data);
					if (item) {
						m_Items[data.objectId] = item;
					}
				}
				else
				{
					// When equip/unequip accessories
					m_Factory.Update(m_Items[data.objectId], data);
				}
				m_NewItems[data.objectId] = data.objectId;
			}
		}

		void OnItemUpdated(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ItemUpdatedEvent::name)
			{
				const auto casted = static_cast<const ItemUpdatedEvent&>(evt);
				const auto& data = casted.GetItemData();

				//todo exception?
				if (m_Items.find(data.objectId) == m_Items.end())
				{
					return;
				}

				m_Factory.Update(m_Items[data.objectId], data);
			}
		}

		void OnItemDeleted(const Event& evt)
		{
			//fixme may be a race condition
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ItemDeletedEvent::name)
			{
				const auto casted = static_cast<const ItemDeletedEvent&>(evt);

				m_Items.erase(casted.GetObjectId());
			}
		}

		ItemRepository() = delete;
		virtual ~ItemRepository()
		{
			Reset();
		}

		void Reset() override
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			m_IsNewCycle = false;
			m_NewItems.clear();
			m_Items.clear();
			m_NetworkHandler.RequestItemList();
		}

	private:
		const ItemFactory& m_Factory;
		std::unordered_map<std::uint32_t, std::shared_ptr<Entities::BaseItem>> m_Items;
		std::unordered_map<std::uint32_t, std::uint32_t> m_NewItems;
		bool m_IsNewCycle = true;
		std::uint32_t m_UsedSkillId = 0;
		const NetworkHandlerWrapper& m_NetworkHandler;
		std::shared_timed_mutex m_Mutex;
	};
}