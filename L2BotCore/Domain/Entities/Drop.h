#pragma once
#include <cstdint>
#include <string>
#include "WorldObject.h"
#include "../DTO/Drop.h"

namespace L2Bot::Domain::Entities
{
	class Drop : public WorldObject
	{
	public:
		const uint32_t GetItemId() const
		{
			return m_ItemId;
		}
		const uint32_t GetAmount() const
		{
			return m_Amount;
		}
		const std::string GetName() const
		{
			return m_Name;
		}
		const std::string GetIconName() const
		{
			return m_IconName;
		}
		void UpdateFromDTO(const DTO::WorldObject* dto) override
		{
			const DTO::Drop* castedDto = static_cast<const DTO::Drop*>(dto);
			WorldObject::UpdateFromDTO(dto);
			m_ItemId = castedDto->itemId;
			m_Amount = castedDto->amount;
			m_Name = castedDto->name;
			m_IconName = castedDto->iconName;
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_IsNewState = false;
		}
		const static Drop CreateFromDTO(const DTO::Drop& dto)
		{
			return Drop(dto.id, dto.transform, dto.itemId, dto.amount, dto.name, dto.iconName);
		}
		const bool IsEqual(const DTO::WorldObject* dto) const override
		{
			const DTO::Drop* castedDto = static_cast<const DTO::Drop*>(dto);
			return WorldObject::IsEqual(dto) &&
				m_ItemId == castedDto->itemId &&
				m_Amount == castedDto->amount &&
				m_Name == castedDto->name &&
				m_IconName == castedDto->iconName;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			if (m_IsNewState)
			{
				result.push_back({ "itemId", std::to_string(m_ItemId) });
				result.push_back({ "amount", std::to_string(m_Amount) });
				result.push_back({ "name", m_Name });
				result.push_back({ "iconName", m_IconName });
			}

			return result;
		}

		Drop(
			const uint32_t id,
			const ValueObjects::Transform transform,
			const uint32_t itemId,
			const uint32_t amount,
			const std::string name,
			const std::string iconName
		) :
			WorldObject(id, transform),
			m_ItemId(itemId),
			m_Amount(amount),
			m_Name(name),
			m_IconName(iconName)
		{
			
		}

		Drop() = default;
		virtual ~Drop() = default;
	private:
		uint32_t m_ItemId = 0;
		uint32_t m_Amount = 0;
		std::string m_Name = "";
		std::string m_IconName = "";
		bool m_IsNewState = true;
	};
}
