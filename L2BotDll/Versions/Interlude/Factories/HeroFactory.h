#pragma once

#include <memory>
#include <format>
#include "../GameStructs/GameStructs.h"
#include "../../../Common/Common.h"
#include "Domain/Entities/Hero.h"
#include "Domain/Exceptions.h"

namespace Interlude
{
	class HeroFactory
	{
	private:
		struct Data
		{
			uint32_t id;
			ValueObjects::Transform transform;
			ValueObjects::FullName fullName;
			ValueObjects::VitalStats vitalStats;
			ValueObjects::Phenotype phenotype;
			ValueObjects::ExperienceInfo experienceInfo;
			ValueObjects::PermanentStats permanentStats;
			ValueObjects::VariableStats variableStats;
			ValueObjects::Reputation reputation;
			ValueObjects::InventoryInfo inventoryInfo;
			uint32_t targetId;
			bool isStanding;
		};

	public:
		HeroFactory() = default;
		virtual ~HeroFactory() = default;

		std::shared_ptr<Entities::Hero> Create(const User* item) const
		{
			const auto& data = GetData(item);

			return std::make_shared<Entities::Hero>(
				data.id,
				data.transform,
				data.fullName,
				data.vitalStats,
				data.phenotype,
				data.experienceInfo,
				data.permanentStats,
				data.variableStats,
				data.reputation,
				data.inventoryInfo,
				data.targetId,
				data.isStanding
			);
		}

		void Update(std::shared_ptr<Entities::Hero>& hero, const User* item) const
		{
			const auto& data = GetData(item);

			hero->Update(
				data.transform,
				data.fullName,
				data.vitalStats,
				data.phenotype,
				data.experienceInfo,
				data.permanentStats,
				data.variableStats,
				data.reputation,
				data.inventoryInfo,
				data.targetId,
				data.isStanding
			);
		}

	private:
		const Data GetData(const User* item) const
		{
			if (!item->pawn) {
				throw RuntimeException(std::format(L"pawn is empty for hero {}", item->nickname));
			}
			const auto playerController = item->pawn->lineagePlayerController;
			if (!playerController) {
				throw RuntimeException(std::format(L"player controller is empty for hero {}", item->nickname));
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
				playerController->targetObjectId,
				playerController->isStanding == 1
			};
		}
	};
}