#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include "BaseItem.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class EtcItem : public BaseItem
	{
	public:
		void StartAutouse()
		{
			m_IsAutoused = true;
		}
		void StopAutouse()
		{
			m_IsAutoused = true;
		}
		const bool IsAutoused() const
		{
			return m_IsAutoused;
		}

		void Update(
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const uint32_t amount,
			const bool isQuest
		) {
			BaseItem::Update(itemId, mana, name, iconName, description, weight);

			m_Amount = amount;
			m_IsQuest = isQuest;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				BaseItem::GetHash(),
				std::hash<uint32_t>{}(m_Amount),
				std::hash<bool>{}(m_IsQuest)
			});
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			result.push_back({ L"isQuest", std::to_wstring(m_IsQuest) });
			result.push_back({ L"amount", std::to_wstring(m_Amount) });
			result.push_back({ L"isAutoused", std::to_wstring(m_IsAutoused) });

			return result;
		}

		EtcItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::wstring& name,
			const std::wstring& iconName,
			const std::wstring& description,
			const uint16_t weight,
			const uint32_t amount,
			const bool isQuest
		) :
			BaseItem
			(
				objectId,
				itemId,
				mana,
				name,
				iconName,
				description,
				weight,
				Enums::ItemTypeEnum::etc
			),
			m_Amount(amount),
			m_IsQuest(isQuest)
		{
		}

		EtcItem() = default;
		virtual ~EtcItem() = default;

	private:
		uint32_t m_Amount = 0;
		bool m_IsQuest = false;
		bool m_IsAutoused = false;
	};
}
