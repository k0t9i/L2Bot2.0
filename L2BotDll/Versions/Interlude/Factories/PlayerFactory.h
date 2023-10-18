#pragma once

#include <memory>
#include <format>
#include "../../../Common/Common.h"
#include "Domain/Entities/Player.h"
#include "Domain/Exceptions.h"

namespace Interlude
{
	class PlayerFactory
	{
	private:
		struct Data
		{
			uint32_t id;
			ValueObjects::Transform transform;
			ValueObjects::FullName fullName;
			ValueObjects::Phenotype phenotype;
			ValueObjects::VitalStats vitalStats;
		};

	public:
		PlayerFactory() = default;
		virtual ~PlayerFactory() = default;

		std::shared_ptr<Entities::Player> Create(const User* item) const
		{
			const auto& data = GetData(item);

			return std::make_shared<Entities::Player>(
				data.id,
				data.transform,
				data.fullName,
				data.phenotype,
				data.vitalStats
			);
		}

		void Update(std::shared_ptr<Entities::Player> &player, const User* item) const
		{
			const auto& data = GetData(item);

			player->Update(
				data.transform,
				data.fullName,
				data.phenotype,
				data.vitalStats
			);
		}

	private:
		const Data GetData(const User* item) const
		{
			if (!item->pawn) {
				throw RuntimeException(std::format(L"pawn is empty for player {}", item->nickname));
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
				ValueObjects::FullName(
					std::wstring(item->nickname),
					std::wstring(item->title)
				),
				ValueObjects::Phenotype(
					(Enums::RaceEnum)item->raceId,
					item->gender == L2::Gender::MALE,
					(Enums::ClassEnum)item->classId,
					(Enums::ClassEnum)item->activeClassId
				),
				ValueObjects::VitalStats(
					item->maxHp, item->hp,
					item->maxMp, item->mp,
					item->maxCp, item->cp
				)
			};
		}
	};
}