#pragma once
#include <cstdint>
#include <string>
#include "WorldObject.h"

namespace L2Bot::Domain::Entities
{
	class Drop : public WorldObject
	{
	public:
		void Update(
			const ValueObjects::Transform &transform,
			const uint32_t itemId,
			const uint32_t amount,
			const std::wstring& name,
			const std::wstring& iconName
		) {
			WorldObject::Update(transform);
			m_ItemId = itemId;
			m_Amount = amount;
			m_Name = name;
			m_IconName = iconName;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				WorldObject::GetHash(),
				std::hash<uint32_t>{}(m_ItemId),
				std::hash<uint32_t>{}(m_Amount),
				std::hash<std::wstring>{}(m_Name),
				std::hash<std::wstring>{}(m_IconName)
			});
		}
		const std::string GetEntityName() const override
		{
			return "drop";
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			result.push_back({ L"itemId", std::to_wstring(m_ItemId) });
			result.push_back({ L"amount", std::to_wstring(m_Amount) });
			result.push_back({ L"name", m_Name });
			result.push_back({ L"iconName", m_IconName });

			return result;
		}

		Drop(
			const uint32_t id,
			const ValueObjects::Transform &transform,
			const uint32_t itemId,
			const uint32_t amount,
			const std::wstring& name,
			const std::wstring& iconName
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
		std::wstring m_Name = L"";
		std::wstring m_IconName = L"";
	};
}
