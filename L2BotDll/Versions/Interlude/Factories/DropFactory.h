#pragma once

#include <memory>
#include "../GameStructs/L2GameDataWrapper.h"
#include "../GameStructs/FName.h"
#include "../GameStructs/GameStructs.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Drop.h"

namespace Interlude
{
	class DropFactory
	{
	public:
		DropFactory(const L2GameDataWrapper& l2GameData, const FName& fName) :
			m_L2GameData(l2GameData),
			m_FName(fName)
		{
		}

		DropFactory() = delete;
		virtual ~DropFactory() = default;

		std::unique_ptr<Entities::EntityInterface> Create(const Item* item) const
		{
			const auto itemData = m_L2GameData.GetItemData(item->itemId);
			const auto nameEntry = itemData ? m_FName.GetEntry(itemData->nameIndex) : nullptr;
			const auto iconEntry = itemData ? m_FName.GetEntry(itemData->iconNameIndex) : nullptr;

			return std::make_unique<Entities::Drop>(
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
				nameEntry ? ConvertFromWideChar(nameEntry->value) : "",
				iconEntry ? ConvertFromWideChar(iconEntry->value) : ""
			);
		}

	private:
		const L2GameDataWrapper& m_L2GameData;
		const FName& m_FName;
	};
}