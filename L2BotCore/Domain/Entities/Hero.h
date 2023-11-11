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
		void Update(
			const ValueObjects::Transform& transform,
			const ValueObjects::FullName& fullName,
			const ValueObjects::VitalStats& vitalStats,
			const ValueObjects::Phenotype& phenotype,
			const ValueObjects::ExperienceInfo& experienceInfo,
			const ValueObjects::PermanentStats& permanentStats,
			const ValueObjects::VariableStats& variableStats,
			const ValueObjects::Reputation& reputation,
			const ValueObjects::InventoryInfo& inventoryInfo,
			const uint32_t targetId,
			const bool isStanding
		) {
			WorldObject::Update(transform);

			m_FullName = fullName;
			m_VitalStats = vitalStats;
			m_Phenotype = phenotype;
			m_ExperienceInfo = experienceInfo;
			m_PermanentStats = permanentStats;
			m_VariableStats = variableStats;
			m_Reputation = reputation;
			m_InventoryInfo = inventoryInfo;
			m_TargetId = targetId;
			m_IsStanding = isStanding;
		}
		const size_t GetHash() const override
		{
			size_t attackersHash = 0;
			for (const auto& kvp : m_AttackerIds)
			{
				attackersHash = Helpers::CombineHashes({
					std::hash<uint32_t>{}(kvp.first),
				});
			}

			return Helpers::CombineHashes({
				WorldObject::GetHash(),
				m_FullName.GetHash(),
				m_VitalStats.GetHash(),
				m_Phenotype.GetHash(),
				m_ExperienceInfo.GetHash(),
				m_PermanentStats.GetHash(),
				m_VariableStats.GetHash(),
				m_Reputation.GetHash(),
				m_InventoryInfo.GetHash(),
				std::hash<uint32_t>{}(m_TargetId),
				std::hash<uint32_t>{}(m_IsStanding),
				attackersHash
			});
		}
		const std::string GetEntityName() const override
		{
			return "hero";
		}
		const ValueObjects::FullName& GetFullName() const
		{
			return m_FullName;
		}
		void AddAttacker(const uint32_t attackerId)
		{
			m_AttackerIds[attackerId] = attackerId;
		}
		void RemoveAttacker(const uint32_t attackerId)
		{
			if (m_AttackerIds.find(attackerId) != m_AttackerIds.end())
			{
				m_AttackerIds.erase(attackerId);
			}
		}
		const std::map<uint32_t, uint32_t>& GetAttackerIds() const
		{
			return m_AttackerIds;
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			result.push_back({ L"fullName", m_FullName.BuildSerializationNodes() });
			result.push_back({ L"vitalStats", m_VitalStats.BuildSerializationNodes() });
			result.push_back({ L"phenotype", m_Phenotype.BuildSerializationNodes() });
			result.push_back({ L"experienceInfo", m_ExperienceInfo.BuildSerializationNodes() });
			result.push_back({ L"permanentStats", m_PermanentStats.BuildSerializationNodes() });
			result.push_back({ L"variableStats", m_VariableStats.BuildSerializationNodes() });
			result.push_back({ L"reputation", m_Reputation.BuildSerializationNodes() });
			result.push_back({ L"inventoryInfo", m_InventoryInfo.BuildSerializationNodes() });
			result.push_back({ L"targetId", std::to_wstring(m_TargetId) });
			result.push_back({ L"isStanding", std::to_wstring(m_IsStanding) });
			std::vector<Serializers::Node> attackers;
			for (const auto& kvp : m_AttackerIds)
			{
				attackers.push_back({ std::to_wstring(kvp.first), std::to_wstring(kvp.first) });
			}
			result.push_back({ L"attackerIds", attackers, true });

			return result;
		}

		Hero(
			const uint32_t id,
			const ValueObjects::Transform& transform,
			const ValueObjects::FullName& fullName,
			const ValueObjects::VitalStats& vitalStats,
			const ValueObjects::Phenotype& phenotype,
			const ValueObjects::ExperienceInfo& experienceInfo,
			const ValueObjects::PermanentStats& permanentStats,
			const ValueObjects::VariableStats& variableStats,
			const ValueObjects::Reputation& reputation,
			const ValueObjects::InventoryInfo& inventoryInfo,
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
		std::map<uint32_t, uint32_t> m_AttackerIds;
	};
}
