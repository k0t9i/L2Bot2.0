#pragma once

#include <memory>
#include <format>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../GameStructs/GameStructs.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Drop.h"
#include "Domain/Exceptions.h"

namespace Interlude
{
	class DropFactory
	{
	private:
		struct Data
		{
			uint32_t id;
			ValueObjects::Transform transform;
			uint32_t itemId;
			uint32_t amount;
			std::wstring name;
			std::wstring iconName;
		};

	public:
		DropFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		DropFactory() = delete;
		virtual ~DropFactory() = default;

		std::shared_ptr<Entities::Drop> Create(const Item* item) const
		{
			const auto &data = GetData(item);

			return std::make_shared<Entities::Drop>(
				data.id,
				data.transform,
				data.itemId,
				data.amount,
				data.name,
				data.iconName
			);
		}

		void Update(std::shared_ptr<Entities::Drop>& drop, const Item* item) const
		{
			const auto& data = GetData(item);

			drop->Update(
				data.transform,
				data.itemId,
				data.amount,
				data.name,
				data.iconName
			);
		}

	private:
		const Data GetData(const Item* item) const
		{
			const auto itemData = m_L2GameData.GetItemData(item->itemId);
			if (!itemData) {
				throw RuntimeException(std::format(L"cannot load ItemData for drop {}", item->itemId));
			}
			const auto nameEntry = m_FName.GetEntry(itemData->nameIndex);
			const auto iconEntry = m_FName.GetEntry(itemData->iconNameIndex);

			if (!item->pawn) {
				throw RuntimeException(std::format(L"pawn is empty for drop {}", std::wstring(nameEntry->value)));
			}

			return {
				item->objectId,
				ValueObjects::Transform(
					ValueObjects::Vector3(item->pawn->Location.x, item->pawn->Location.y, item->pawn->Location.z),
					ValueObjects::Vector3(
						static_cast<float_t>(item->pawn->Rotation.Pitch),
						static_cast<float_t>(item->pawn->Rotation.Yaw),
						static_cast<float_t>(item->pawn->Rotation.Roll)
					),
					ValueObjects::Vector3(item->pawn->Velocity.x, item->pawn->Velocity.y, item->pawn->Velocity.z),
					ValueObjects::Vector3(item->pawn->Acceleration.x, item->pawn->Acceleration.y, item->pawn->Acceleration.z)
				),
				item->itemId,
				item->amount,
				nameEntry ? std::wstring(nameEntry->value) : L"",
				iconEntry ? std::wstring(iconEntry->value) : L""
			};
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}