#pragma once

#include <map>
#include <chrono>
#include <shared_mutex>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "../Factories/ItemFactory.h"
#include "../GameStructs/NetworkHandlerWrapper.h"
#include "../../../Services/EntityHandler.h"
#include "../../../Events/ItemCreatedEvent.h"
#include "../../../Events/ItemUpdatedEvent.h"
#include "../../../Events/ItemDeletedEvent.h"
#include "../../../Events/HeroDeletedEvent.h"
#include "../../../Events/EventDispatcher.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class ItemRepository : public Repositories::EntityRepositoryInterface
	{
	public:
		const std::vector<std::shared_ptr<DTO::EntityState>> GetEntities() override
		{
			std::unique_lock<std::shared_timed_mutex>(m_Mutex);

			std::map<uint32_t, Entities::BaseItem*> itemPtrs;
			for (const auto& kvp : m_Items)
			{
				itemPtrs[kvp.first] = kvp.second.get();
			}

			const auto objects = m_EntityHandler.GetEntities<Entities::BaseItem*>(itemPtrs, [this](Entities::BaseItem* item) {
				return m_Factory.CreateFromPointer(item);
			});

			auto result = std::vector<std::shared_ptr<DTO::EntityState>>();

			for (const auto kvp : objects)
			{
				result.push_back(kvp.second);
			}

			return result;
		}

		ItemRepository(const NetworkHandlerWrapper& networkHandler, const ItemFactory& factory, EntityHandler& handler) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_EntityHandler(handler)
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
		}

		void OnHeroDeleted(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == HeroDeletedEvent::name)
			{
				Reset();
			}
		}

		void OnItemCreated(const Event& evt)
		{
			std::shared_lock<std::shared_timed_mutex>(m_Mutex);
			if (evt.GetName() == ItemCreatedEvent::name)
			{
				const auto casted = static_cast<const ItemCreatedEvent&>(evt);
				const auto& data = casted.GetItemData();

				auto item = m_Factory.Create(data);
				if (m_Items.find(data.objectId) == m_Items.end())
				{
					m_Items.emplace(data.objectId, std::move(item));
				}
				else
				{
					// When equip/unequip accessories
					m_Items[data.objectId]->Update(item.get());
				}
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

				auto item = m_Factory.Create(data);
				m_Items[data.objectId]->Update(item.get());
			}
		}

		//todo deleted ehchant scroll
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
			m_Items.clear();
		}

	private:
		const ItemFactory& m_Factory;
		std::map<uint32_t, std::unique_ptr<Entities::BaseItem>> m_Items;
		uint32_t m_UsedSkillId = 0;
		const NetworkHandlerWrapper& m_NetworkHandler;
		std::shared_timed_mutex m_Mutex;
		EntityHandler& m_EntityHandler;
	};
}