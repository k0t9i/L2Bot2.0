#pragma once
#include "WorldObject.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../ValueObjects/Phenotype.h"

namespace L2Bot::Domain::Entities
{
	class Player : public WorldObject
	{
	public:
		void Update(
			const ValueObjects::Transform& transform,
			const ValueObjects::FullName& fullName,
			const ValueObjects::Phenotype& phenotype,
			const ValueObjects::VitalStats& vitalStats
		) {
			WorldObject::Update(transform);

			m_FullName = fullName;
			m_Phenotype = phenotype;
			m_VitalStats.LoadFromOther(vitalStats);
		}
		const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				WorldObject::GetHash(),
				m_FullName.GetHash(),
				m_Phenotype.GetHash(),
				m_VitalStats.GetHash()
			});
		}
		const std::string GetEntityName() const override
		{
			return "player";
		}
		void MarkAsDead()
		{
			m_VitalStats.MarkAsDead();
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			result.push_back({ L"fullName", m_FullName.BuildSerializationNodes() });
			result.push_back({ L"phenotype", m_Phenotype.BuildSerializationNodes() });
			result.push_back({ L"vitalStats", m_VitalStats.BuildSerializationNodes() });

			return result;
		}

		Player(
			const uint32_t id,
			const ValueObjects::Transform& transform,
			const ValueObjects::FullName& fullName,
			const ValueObjects::Phenotype& phenotype,
			const ValueObjects::VitalStats& vitalStats
		) :
			WorldObject(id, transform),
			m_FullName(fullName),
			m_Phenotype(phenotype),
			m_VitalStats(vitalStats)
		{
		}

		Player() = default;
		virtual ~Player() = default;

	private:
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::Phenotype m_Phenotype = ValueObjects::Phenotype();
		ValueObjects::VitalStats m_VitalStats = ValueObjects::VitalStats();
	};
}
