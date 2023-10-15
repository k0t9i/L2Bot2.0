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
		void UpdateSpoilState(const Enums::SpoilStateEnum spoilState)
		{
			m_SpoilState = spoilState;
		}

		void Update(
			const ValueObjects::Transform& transform,
			const bool isHostile,
			const uint32_t npcId,
			const ValueObjects::FullName& fullName,
			const ValueObjects::VitalStats& vitalStats
		) {
			WorldObject::Update(transform);

			m_IsHostile = isHostile;
			m_NpcId = npcId;
			m_FullName = fullName;
			m_VitalStats = vitalStats;
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				WorldObject::GetHash(),
				std::hash<bool>{}(m_IsHostile),
				std::hash<uint32_t>{}(m_NpcId),
				std::hash<Enums::SpoilStateEnum>{}(m_SpoilState),
				m_FullName.GetHash(),
				m_VitalStats.GetHash()
			});
		}
		const std::string GetEntityName() const override
		{
			return "npc";
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			result.push_back({ L"fullName", m_FullName.BuildSerializationNodes() });
			result.push_back({ L"isHostile", std::to_wstring(m_IsHostile) });
			result.push_back({ L"npcId", std::to_wstring(m_NpcId) });
			result.push_back({ L"spoilState", std::to_wstring(static_cast<uint32_t>(m_SpoilState)) });
			result.push_back({ L"vitalStats", m_VitalStats.BuildSerializationNodes() });

			return result;
		}


		NPC(
			const uint32_t id,
			const ValueObjects::Transform& transform,
			const bool isHostile,
			const uint32_t npcId,
			const ValueObjects::FullName& fullName,
			const ValueObjects::VitalStats& vitalStats
		) :
			WorldObject(id, transform),
			m_IsHostile(isHostile),
			m_NpcId(npcId),
			m_FullName(fullName),
			m_VitalStats(vitalStats)
		{

		}

		NPC() = default;
		virtual ~NPC() = default;

	private:
		bool m_IsHostile = false;
		uint32_t m_NpcId = 0;
		Enums::SpoilStateEnum m_SpoilState = Enums::SpoilStateEnum::none;
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::VitalStats m_VitalStats = ValueObjects::VitalStats();
	};
}
