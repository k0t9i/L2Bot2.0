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
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_PrevState =
			{
				m_FullName,
				m_Phenotype,
				false
			};
		}
		const bool IsEqual(const EntityInterface* other) const override
		{
			const Player* casted = static_cast<const Player*>(other);
			return WorldObject::IsEqual(other) &&
				m_FullName.IsEqual(&casted->m_FullName) &&
				m_Phenotype.IsEqual(&casted->m_Phenotype);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result = WorldObject::BuildSerializationNodes();

			if (m_PrevState.isNewState || !m_FullName.IsEqual(&m_PrevState.fullName))
			{
				result.push_back({ "fullName", m_FullName.BuildSerializationNodes() });
			}
			if (m_PrevState.isNewState || !m_Phenotype.IsEqual(&m_PrevState.phenotype))
			{
				result.push_back({ "phenotype", m_Phenotype.BuildSerializationNodes() });
			}

			return result;
		}

		Player(
			const uint32_t id,
			const ValueObjects::Transform transform,
			const ValueObjects::FullName fullName,
			const ValueObjects::Phenotype phenotype
		) :
			WorldObject(id, transform),
			m_FullName(fullName),
			m_Phenotype(phenotype)
		{
			SaveState();
		}

		Player() = default;
		virtual ~Player() = default;

	private:
		struct GetState
		{
			ValueObjects::FullName fullName = ValueObjects::FullName();
			ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();

			bool isNewState = true;
		};

	private:
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::Phenotype m_Phenotype = ValueObjects::Phenotype();
		GetState m_PrevState = GetState();
	};
}
