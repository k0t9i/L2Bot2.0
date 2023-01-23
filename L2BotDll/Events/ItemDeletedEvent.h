#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"
#include "../DTO/ItemData.h"

class ItemDeletedEvent : public Event
{
public:
	static constexpr const char* name = "itemDeleted";

	const std::string GetName() const
	{
		return std::string(name);
	}

	const uint32_t GetItemId() const
	{
		return m_Id;
	}

	ItemDeletedEvent(const uint32_t id) :
		m_Id(id)
	{

	}

	ItemDeletedEvent() = delete;
	virtual ~ItemDeletedEvent() = default;

private:
	const uint32_t m_Id;
};