#pragma once

#include <memory>
#include "../GameStructs/GameStructs.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Hero.h"

namespace Interlude
{
	class HeroFactory
	{
	public:
		HeroFactory() = default;
		virtual ~HeroFactory() = default;

		std::unique_ptr<Entities::Hero> Create(const User* item) const
		{
			const auto playerController = item->pawn ? item->pawn->lineagePlayerController : nullptr;

			return std::make_unique<Entities::Hero>(
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
					ConvertFromWideChar(item->nickname),
					ConvertFromWideChar(item->title)
				),
				ValueObjects::VitalStats(
					item->maxHp, item->hp,
					item->maxMp, item->mp,
					item->maxCp, item->cp
				),
				ValueObjects::Phenotype(
					(Enums::RaceEnum)item->raceId,
					item->gender == L2::Gender::MALE,
					(Enums::ClassEnum)item->classId,
					(Enums::ClassEnum)item->activeClassId
				),
				ValueObjects::ExperienceInfo(
					item->lvl,
					item->exp,
					item->sp
				),
				ValueObjects::PermanentStats(
					item->str,
					item->dex,
					item->con,
					item->int_,
					item->men,
					item->wit
				),
				ValueObjects::VariableStats(
					item->accuracy,
					item->critRate,
					item->pAttack,
					item->attackSpeed,
					item->pDefense,
					item->evasion,
					item->mAttack,
					item->mDefense,
					item->castingSpeed
				),
				ValueObjects::Reputation(
					item->karma,
					item->pkKills,
					item->pvpKills,
					static_cast<uint8_t>(item->recRemaining),
					static_cast<uint8_t>(item->evalScore)
				),
				ValueObjects::InventoryInfo(
					item->maxWeight,
					item->weight,
					item->invSlotCount
				),
				playerController ? playerController->targetObjectId : 0,
				playerController ? playerController->isStanding == 1 : true
			);
		}
	};
}