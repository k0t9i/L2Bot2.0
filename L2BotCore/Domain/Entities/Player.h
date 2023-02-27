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
		void Update(const EntityInterface* other) override
		{
			const Player* casted = static_cast<const Player*>(other);
			WorldObject::Update(other);
			m_FullName = casted->m_FullName;
			m_Phenotype = casted->m_Phenotype;
			m_VitalStats = casted->m_VitalStats;
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_PrevState =
			{
				m_FullName,
				m_Phenotype,
				m_VitalStats,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const Player* casted = static_cast<const Player*>(other);
			return WorldObject::IsEqual(other) &&
				m_FullName.IsEqual(&casted->m_FullName) &&
				m_Phenotype.IsEqual(&casted->m_Phenotype) &&
				m_VitalStats.IsEqual(&casted->m_VitalStats);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			if (m_PrevState.isNewState || !m_FullName.IsEqual(&m_PrevState.fullName))
			{
				result.push_back({ L"fullName", m_FullName.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_Phenotype.IsEqual(&m_PrevState.phenotype))
			{
				result.push_back({ L"phenotype", m_Phenotype.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_VitalStats.IsEqual(&m_PrevState.vitalStats))
			{
				result.push_back({ L"vitalStats", m_VitalStats.BuildSerializationNodes() });
			}

			return result;
		}

		Player(
			const uint32_t id,
			const ValueObjects::Transform transform,
			const ValueObjects::FullName fullName,
			const ValueObjects::Phenotype phenotype,
			const ValueObjects::VitalStats vitalStats
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
		struct GetState
		{
			ValueObjects::FullName fullName = ValueObjects::FullName();
			ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();
			ValueObjects::VitalStats vitalStats = ValueObjects::VitalStats();

			bool isNewState = true;
		};

	private:
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::Phenotype m_Phenotype = ValueObjects::Phenotype();
		ValueObjects::VitalStats m_VitalStats = ValueObjects::VitalStats();
		GetState m_PrevState = GetState();
	};
}
