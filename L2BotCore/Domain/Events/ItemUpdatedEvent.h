#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ItemData.h"

namespace L2Bot::Domain::Events
{
	class ItemUpdatedEvent : public Event
	{
	public:
		static constexpr const char* name = "itemUpdated";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const DTO::ItemData& GetItemData() const
		{
			return m_ItemData;
		}

		ItemUpdatedEvent(const DTO::ItemData itemData) :
			m_ItemData(itemData)
		{

		}

		ItemUpdatedEvent() = delete;
		virtual ~ItemUpdatedEvent() = default;

	private:
		const DTO::ItemData m_ItemData;
	};
}