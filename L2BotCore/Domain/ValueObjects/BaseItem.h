#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "../DTO/BaseItem.h"
#include "../Serializers/Serializable.h"
#include "../Serializers/Node.h"

namespace L2Bot::Domain::ValueObjects
{
	class BaseItem : public Serializers::Serializable
	{
	public:
		const uint32_t GetId() const
		{
			return m_ItemId;
		}
		void UpdateFromDTO(const DTO::BaseItem* dto)
		{
			SaveState();

			m_ItemId = dto->itemId;
			m_Amount = dto->amount;
			m_IsEquipped = dto->isEquipped;
			m_EnchantLevel = dto->enchantLevel;
			m_Mana = dto->mana;
			m_Name = dto->name;
			m_IconName = dto->iconName;
			m_Description = dto->description;
			m_Weight = dto->weight;
		}
		void SaveState()
		{
			m_PrevState =
			{
				m_Amount,
				m_IsEquipped,
				m_EnchantLevel,
				m_Mana,
				m_Weight,
				false
			};
		}
		const static BaseItem CreateFromDTO(const DTO::BaseItem& dto)
		{
			return BaseItem(
				dto.itemId,
				dto.amount,
				dto.isEquipped,
				dto.enchantLevel,
				dto.mana,
				dto.name,
				dto.iconName,
				dto.description,
				dto.weight
			);
		}
		const bool IsEqual(const DTO::BaseItem* dto) const
		{
			return m_ItemId == dto->itemId &&
				m_Amount == dto->amount &&
				m_IsEquipped == dto->isEquipped &&
				m_EnchantLevel == dto->enchantLevel &&
				m_Mana == dto->mana &&
				m_Name == dto->name &&
				m_IconName == dto->iconName &&
				m_Description == dto->description &&
				m_Weight == dto->weight;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ "itemId", std::to_string(m_ItemId) });

			if (m_PrevState.isNewState)
			{
				result.push_back({ "name", m_Name });
				result.push_back({ "iconName", m_IconName });
				result.push_back({ "description", m_Description });
			}

			if (m_PrevState.isNewState || m_Amount != m_PrevState.amount)
			{
				result.push_back({ "amount", std::to_string(m_Amount) });
			}
			if (m_PrevState.isNewState || m_IsEquipped != m_PrevState.isEquipped)
			{
				result.push_back({ "isEquipped", std::to_string(m_IsEquipped) });
			}
			if (m_PrevState.isNewState || m_EnchantLevel != m_PrevState.enchantLevel)
			{
				result.push_back({ "enchantLevel", std::to_string(m_EnchantLevel) });
			}
			if (m_PrevState.isNewState || m_Mana != m_PrevState.mana)
			{
				result.push_back({ "mana", std::to_string(m_Mana) });
			}
			if (m_PrevState.isNewState || m_Weight != m_PrevState.weight)
			{
				result.push_back({ "weight", std::to_string(m_Weight) });
			}

			return result;
		}

		BaseItem(
			const uint32_t itemId,
			const uint32_t amount,
			const bool isEquipped,
			const uint16_t enchantLevel,
			const int32_t mana,
			const std::string name,
			const std::string iconName,
			const std::string description,
			const uint16_t weight
		) :
			m_ItemId(itemId),
			m_Amount(amount),
			m_IsEquipped(isEquipped),
			m_EnchantLevel(enchantLevel),
			m_Mana(mana),
			m_Name(name),
			m_IconName(iconName),
			m_Description(description),
			m_Weight(weight)
		{
		}

		BaseItem() = default;
		virtual ~BaseItem() = default;

	private:
		struct State
		{
			uint32_t amount = 0;
			bool isEquipped = 0;
			uint16_t enchantLevel = 0;
			int32_t mana = -1;
			uint16_t weight = 0;

			bool isNewState = true;
		};

	private:
		uint32_t m_ItemId = 0;
		uint32_t m_Amount = 0;
		bool m_IsEquipped = 0;
		uint16_t m_EnchantLevel = 0;
		int32_t m_Mana = -1;
		std::string m_Name = "";
		std::string m_IconName = "";
		std::string m_Description = "";
		uint16_t m_Weight = 0;
		State m_PrevState = State();
	};
}
