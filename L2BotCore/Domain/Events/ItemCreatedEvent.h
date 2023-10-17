#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ItemData.h"

namespace L2Bot::Domain::Events
{
	class ItemCreatedEvent : public Event
	{
	public:
		static constexpr const char* name = "itemCreated";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const DTO::ItemData& GetItemData() const
		{
			return m_ItemData;
		}

		ItemCreatedEvent(const DTO::ItemData itemData) :
			m_ItemData(itemData)
		{

		}

		ItemCreatedEvent() = delete;
		virtual ~ItemCreatedEvent() = default;

	private:
		const DTO::ItemData m_ItemData;
	};
}