#pragma once
#include <cstdint>
#include "WorldObject.h"
#include "../DTO/NPC.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../Serializers/Serializable.h"
#include "../Enums/SpoilStateEnum.h"

namespace L2Bot::Domain::Entities
{
	class NPC : public WorldObject
	{
	public:
		const bool IsHostile() const
		{
			return m_IsHostile;
		}
		const uint32_t GetNpcId() const
		{
			return m_NpcId;
		}
		const bool IsSpoiled() const
		{
			return m_SpoilState == Enums::SpoilStateEnum::spoiled;
		}
		const bool CanBeSweeped() const
		{
			return !m_VitalStats.IsAlive() && m_SpoilState == Enums::SpoilStateEnum::sweepable;
		}
		const ValueObjects::FullName& GetFullName() const
		{
			return m_FullName;
		}
		const ValueObjects::VitalStats& GetVitalStats() const
		{
			return m_VitalStats;
		}

		void UpdateFromDTO(const DTO::WorldObject* dto) override
		{
			const DTO::NPC* castedDto = static_cast<const DTO::NPC*>(dto);
			WorldObject::UpdateFromDTO(dto);
			m_IsHostile = castedDto->isHostile;
			m_NpcId = castedDto->npcId;
			m_SpoilState = castedDto->spoilState;
			m_FullName = castedDto->fullName;
			m_VitalStats = castedDto->vitalStats;
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_PrevState =
			{
				m_FullName,
				m_SpoilState,
				m_VitalStats,
				false
			};
		}
		const static NPC CreateFromDTO(const DTO::NPC& dto)
		{
			return NPC(
				dto.id,
				dto.transform,
				dto.isHostile,
				dto.npcId,
				dto.spoilState,
				dto.fullName,
				dto.vitalStats
			);
		}
		const bool IsEqual(const DTO::WorldObject* dto) const override
		{
			const DTO::NPC* castedDto = static_cast<const DTO::NPC*>(dto);
			return WorldObject::IsEqual(dto) &&
				m_IsHostile == castedDto->isHostile &&
				m_NpcId == castedDto->npcId &&
				m_SpoilState == castedDto->spoilState &&
				m_FullName.IsEqual(&castedDto->fullName) &&
				m_VitalStats.IsEqual(&castedDto->vitalStats);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			if (m_PrevState.isNewState || !m_FullName.IsEqual(&m_PrevState.fullName))
			{
				result.push_back({ "fullName", m_FullName.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState)
			{
				result.push_back({ "isHostile", std::to_string(m_IsHostile) });
				result.push_back({ "npcId", std::to_string(m_NpcId) });
			}
			if (m_PrevState.isNewState || m_SpoilState != m_PrevState.spoilState)
			{
				result.push_back({ "spoilState", std::to_string(static_cast<uint32_t>(m_SpoilState)) });
			}
			if (m_PrevState.isNewState || !m_VitalStats.IsEqual(&m_PrevState.vitalStats))
			{
				result.push_back({ "vitalStats", m_VitalStats.BuildSerializationNodes() });
			}

			return result;
		}

		NPC(
			const uint32_t id,
			const ValueObjects::Transform transform,
			const bool isHostile,
			const uint32_t npcId,
			const Enums::SpoilStateEnum spoilState,
			const ValueObjects::FullName fullName,
			const ValueObjects::VitalStats vitalStats
		) :
			WorldObject(id, transform),
			m_IsHostile(isHostile),
			m_NpcId(npcId),
			m_SpoilState(spoilState),
			m_FullName(fullName),
			m_VitalStats(vitalStats)
		{

		}

		NPC() = default;
		virtual ~NPC() = default;

	private:
		struct State
		{
			ValueObjects::FullName fullName = ValueObjects::FullName();
			Enums::SpoilStateEnum spoilState = Enums::SpoilStateEnum::none;
			ValueObjects::VitalStats vitalStats = ValueObjects::VitalStats();

			bool isNewState = true;
		};

	private:
		bool m_IsHostile = false;
		uint32_t m_NpcId = 0;
		Enums::SpoilStateEnum m_SpoilState = Enums::SpoilStateEnum::none;
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::VitalStats m_VitalStats = ValueObjects::VitalStats();
		State m_PrevState = State();
	};
}
