#pragma once

#include <memory>
#include <format>
#include "../../../Common/Common.h"
#include "Domain/Entities/NPC.h"
#include "Domain/Exceptions.h"

namespace Interlude
{
	class NPCFactory
	{
	private:
		struct Data
		{
			uint32_t id;
			ValueObjects::Transform transform;
			bool isHostile;
			uint32_t npcId;
			ValueObjects::FullName fullName;
			ValueObjects::VitalStats vitalStats;
		};

	public:
		NPCFactory() = default;
		virtual ~NPCFactory() = default;

		std::shared_ptr<Entities::NPC> Create(const User* item) const
		{
			const auto& data = GetData(item);

			return std::make_shared<Entities::NPC>(
				data.id,
				data.transform,
				data.isHostile,
				data.npcId,
				data.fullName,
				data.vitalStats
			);
		}

		void Update(std::shared_ptr<Entities::NPC>& npc, const User* item) const
		{
			const auto& data = GetData(item);

			npc->Update(
				data.transform,
				data.isHostile,
				data.npcId,
				data.fullName,
				data.vitalStats
			);
		}

	private:
		const Data GetData(const User* item) const
		{
			if (!item->pawn) {
				throw RuntimeException(std::format(L"pawn is empty for npc {}", item->nickname));
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
				item->isMob != 0,
				item->npcId,
				ValueObjects::FullName(
					std::wstring(item->nickname),
					std::wstring(item->title)
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