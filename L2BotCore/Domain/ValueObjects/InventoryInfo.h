#pragma once
#include <cstdint>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::ValueObjects
{
	class InventoryInfo : public Serializers::Serializable
	{
	public:
		const uint32_t GetMaxWeight() const
		{
			return m_MaxWeight;
		}
		const uint32_t GetWeight() const
		{
			return m_Weight;
		}
		const uint16_t GetSlots() const
		{
			return m_Slots;
		}
		const bool IsEqual(const InventoryInfo* other) const
		{
			return m_MaxWeight == other->m_MaxWeight &&
				m_Weight == other->m_Weight &&
				m_Slots == other->m_Slots;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			return std::vector<Serializers::Node>
			{
				{ L"maxWeight", std::to_wstring(m_MaxWeight) },
				{ L"weight", std::to_wstring(m_Weight) },
				{ L"slots", std::to_wstring(m_Slots) }
			};
		}

		InventoryInfo(
			const uint32_t maxWeight,
			const uint32_t weight,
			const uint16_t slots
		) :
			m_MaxWeight(maxWeight),
			m_Weight(weight),
			m_Slots(slots)
		{
		}

		InventoryInfo() = default;
		virtual ~InventoryInfo() = default;
	private:
		uint32_t m_MaxWeight = 0;
		uint32_t m_Weight = 0;
		uint16_t m_Slots = 0;
	};
}
