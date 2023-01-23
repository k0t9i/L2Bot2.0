#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ItemData.h"

class ItemUpdatedEvent : public Event
{
public:
	static constexpr const char* name = "itemUpdated";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const ItemData& GetItemData() const
	{
		return m_ItemData;
	}

	ItemUpdatedEvent(const ItemData& itemData) :
		m_ItemData(itemData)
	{

	}

	ItemUpdatedEvent() = delete;
	virtual ~ItemUpdatedEvent() = default;

private:
	const ItemData m_ItemData;
};