#pragma once
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../ValueObjects/Phenotype.h"
#include "../ValueObjects/ExperienceInfo.h"
#include "../ValueObjects/PermanentStats.h"
#include "../ValueObjects/VariableStats.h"
#include "../ValueObjects/Reputation.h"
#include "../ValueObjects/InventoryInfo.h"
#include <cstdint>

namespace L2Bot::Domain::Entities
{
	class Hero : public WorldObject
	{
	public:
		void Update(const EntityInterface* other) override
		{
			const Hero* casted = static_cast<const Hero*>(other);
			WorldObject::Update(other);
			m_FullName = casted->m_FullName;
			m_VitalStats = casted->m_VitalStats;
			m_Phenotype = casted->m_Phenotype;
			m_ExperienceInfo = casted->m_ExperienceInfo;
			m_PermanentStats = casted->m_PermanentStats;
			m_VariableStats = casted->m_VariableStats;
			m_Reputation = casted->m_Reputation;
			m_InventoryInfo = casted->m_InventoryInfo;
			m_TargetId = casted->m_TargetId;
			m_IsStanding = casted->m_IsStanding;
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_PrevState =
			{
				m_FullName,
				m_VitalStats,
				m_Phenotype,
				m_ExperienceInfo,
				m_PermanentStats,
				m_VariableStats,
				m_Reputation,
				m_InventoryInfo,
				m_TargetId,
				m_IsStanding,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const Hero* casted = static_cast<const Hero*>(other);
			return WorldObject::IsEqual(other) &&
				m_FullName.IsEqual(&casted->m_FullName) &&
				m_VitalStats.IsEqual(&casted->m_VitalStats) &&
				m_Phenotype.IsEqual(&casted->m_Phenotype) &&
				m_ExperienceInfo.IsEqual(&casted->m_ExperienceInfo) &&
				m_PermanentStats.IsEqual(&casted->m_PermanentStats) &&
				m_VariableStats.IsEqual(&casted->m_VariableStats) &&
				m_Reputation.IsEqual(&casted->m_Reputation) &&
				m_InventoryInfo.IsEqual(&casted->m_InventoryInfo) &&
				m_TargetId == casted->m_TargetId &&
				m_IsStanding == casted->m_IsStanding;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			if (m_PrevState.isNewState || !m_FullName.IsEqual(&m_PrevState.fullName))
			{
				result.push_back({ "fullName", m_FullName.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_VitalStats.IsEqual(&m_PrevState.vitalStats))
			{
				result.push_back({ "vitalStats", m_VitalStats.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_Phenotype.IsEqual(&m_PrevState.phenotype))
			{
				result.push_back({ "phenotype", m_Phenotype.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_ExperienceInfo.IsEqual(&m_PrevState.experienceInfo))
			{
				result.push_back({ "experienceInfo", m_ExperienceInfo.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_PermanentStats.IsEqual(&m_PrevState.permanentStats))
			{
				result.push_back({ "permanentStats", m_PermanentStats.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_VariableStats.IsEqual(&m_PrevState.variableStats))
			{
				result.push_back({ "variableStats", m_VariableStats.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_Reputation.IsEqual(&m_PrevState.reputation))
			{
				result.push_back({ "reputation", m_Reputation.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_InventoryInfo.IsEqual(&m_PrevState.inventoryInfo))
			{
				result.push_back({ "inventoryInfo", m_InventoryInfo.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || m_TargetId != m_PrevState.targetId)
			{
				result.push_back({ "targetId", std::to_string(m_TargetId) });
			}
			if (m_PrevState.isNewState || m_IsStanding != m_PrevState.isStanding)
			{
				result.push_back({ "isStanding", std::to_string(m_IsStanding) });
			}

			return result;
		}

		Hero(
			const uint32_t id,
			const ValueObjects::Transform transform,
			const ValueObjects::FullName fullName,
			const ValueObjects::VitalStats vitalStats,
			const ValueObjects::Phenotype phenotype,
			const ValueObjects::ExperienceInfo experienceInfo,
			const ValueObjects::PermanentStats permanentStats,
			const ValueObjects::VariableStats variableStats,
			const ValueObjects::Reputation reputation,
			const ValueObjects::InventoryInfo inventoryInfo,
			const uint32_t targetId,
			const bool isStanding
		) :
			WorldObject(id, transform),
			m_FullName(fullName),
			m_VitalStats(vitalStats),
			m_Phenotype(phenotype),
			m_ExperienceInfo(experienceInfo),
			m_PermanentStats(permanentStats),
			m_VariableStats(variableStats),
			m_Reputation(reputation),
			m_InventoryInfo(inventoryInfo),
			m_TargetId(targetId),
			m_IsStanding(isStanding)
		{

		}

		Hero() = default;
		virtual ~Hero() = default;

	private:
		struct GetState
		{
			ValueObjects::FullName fullName = ValueObjects::FullName();
			ValueObjects::VitalStats vitalStats = ValueObjects::VitalStats();
			ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();
			ValueObjects::ExperienceInfo experienceInfo = ValueObjects::ExperienceInfo();
			ValueObjects::PermanentStats permanentStats = ValueObjects::PermanentStats();
			ValueObjects::VariableStats variableStats = ValueObjects::VariableStats();
			ValueObjects::Reputation reputation = ValueObjects::Reputation();
			ValueObjects::InventoryInfo inventoryInfo = ValueObjects::InventoryInfo();
			uint32_t targetId = 0;
			bool isStanding = true;

			bool isNewState = true;
		};

	private:
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::VitalStats m_VitalStats = ValueObjects::VitalStats();
		ValueObjects::Phenotype m_Phenotype = ValueObjects::Phenotype();
		ValueObjects::ExperienceInfo m_ExperienceInfo = ValueObjects::ExperienceInfo();
		ValueObjects::PermanentStats m_PermanentStats = ValueObjects::PermanentStats();
		ValueObjects::VariableStats m_VariableStats = ValueObjects::VariableStats();
		ValueObjects::Reputation m_Reputation = ValueObjects::Reputation();
		ValueObjects::InventoryInfo m_InventoryInfo = ValueObjects::InventoryInfo();
		uint32_t m_TargetId = 0;
		bool m_IsStanding = true;
		GetState m_PrevState = GetState();
	};
}
