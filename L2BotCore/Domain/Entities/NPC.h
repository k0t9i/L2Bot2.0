#pragma once
#include <cstdint>
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../Serializers/Serializable.h"
#include "../Enums/SpoilStateEnum.h"

namespace L2Bot::Domain::Entities
{
	class NPC : public WorldObject
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const NPC* casted = static_cast<const NPC*>(other);
			WorldObject::Update(other);
			m_IsHostile = casted->m_IsHostile;
			m_NpcId = casted->m_NpcId;
			m_SpoilState = casted->m_SpoilState;
			m_FullName = casted->m_FullName;
			m_VitalStats = casted->m_VitalStats;
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
		const bool IsEqual(const EntityInterface* other) const override
		{
			const NPC* casted = static_cast<const NPC*>(other);
			return WorldObject::IsEqual(other) &&
				m_IsHostile == casted->m_IsHostile &&
				m_NpcId == casted->m_NpcId &&
				m_SpoilState == casted->m_SpoilState &&
				m_FullName.IsEqual(&casted->m_FullName) &&
				m_VitalStats.IsEqual(&casted->m_VitalStats);
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
		struct GetState
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
		GetState m_PrevState = GetState();
	};
}
