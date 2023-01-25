#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "BaseItem.h"

namespace L2Bot::Domain::Entities
{
	class EtcItem : public BaseItem
	{
	public:
		void Autouse(bool enabled)
		{
			m_IsAutoused = enabled;
		}

		void Update(const EntityInterface* other) override
		{
			const EtcItem* casted = static_cast<const EtcItem*>(other);
			
			BaseItem::Update(other);

			m_Amount = casted->m_Amount;
			m_IsQuest = casted->m_IsQuest;
			m_IsAutoused = casted->m_IsAutoused;
		}
		void SaveState() override
		{
			BaseItem::SaveState();
			m_PrevState =
			{
				m_Amount,
				m_IsAutoused,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const EtcItem* casted = static_cast<const EtcItem*>(other);
			return BaseItem::IsEqual(other) &&
				m_IsQuest == casted->m_IsQuest &&
				m_Amount == casted->m_Amount &&
				m_IsAutoused == casted->m_IsAutoused;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = BaseItem::BuildSerializationNodes();

			if (m_PrevState.isNewState)
			{
				result.push_back({ "isQuest", std::to_string(m_IsQuest) });
			}

			if (m_PrevState.isNewState || m_Amount != m_PrevState.amount)
			{
				result.push_back({ "amount", std::to_string(m_Amount) });
			}
			if (m_PrevState.isNewState || m_IsAutoused != m_PrevState.isAutoused)
			{
				result.push_back({ "isAutoused", std::to_string(m_IsAutoused) });
			}

			return result;
		}

		EtcItem(
			const uint32_t objectId,
			const uint32_t itemId,
			const int32_t mana,
			const std::string& name,
			const std::string& iconName,
			const std::string& description,
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

		EtcItem(const EtcItem* other) :
			BaseItem(other),
			m_Amount(other->m_Amount),
			m_IsAutoused(other->m_IsAutoused)
		{
		}

		EtcItem() = default;
		virtual ~EtcItem() = default;

	private:
		struct GetState
		{
			uint32_t amount = 0;
			bool isAutoused = false;

			bool isNewState = true;
		};

	private:
		uint32_t m_Amount = 0;
		bool m_IsQuest = false;
		bool m_IsAutoused = false;
		GetState m_PrevState = GetState();
	};
}
