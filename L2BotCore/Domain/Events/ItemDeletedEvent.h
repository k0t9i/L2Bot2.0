#pragma once

#include <cstdint>
#include <vector>
#include "Event.h"

namespace L2Bot::Domain::Events
{
	class ItemDeletedEvent : public Event
	{
	public:
		static constexpr const char* name = "itemDeleted";

		const std::string GetName() const
		{
			return std::string(name);
		}

		const uint32_t GetObjectId() const
		{
			return m_ObjectId;
		}

		ItemDeletedEvent(const uint32_t objectId) :
			m_ObjectId(objectId)
		{

		}

		ItemDeletedEvent() = delete;
		virtual ~ItemDeletedEvent() = default;

	private:
		const uint32_t m_ObjectId;
	};
}