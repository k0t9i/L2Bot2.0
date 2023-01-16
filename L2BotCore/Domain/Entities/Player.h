#pragma once
#include "WorldObject.h"
#include "../DTO/Player.h"
#include "../ValueObjects/FullName.h"
#include "../ValueObjects/VitalStats.h"
#include "../ValueObjects/Phenotype.h"

namespace L2Bot::Domain::Entities
{
	class Player : public WorldObject
	{
	public:
		const ValueObjects::FullName& GetFullName() const
		{
			return m_FullName;
		}
		const ValueObjects::Phenotype& GetPhenotype() const
		{
			return m_Phenotype;
		}

		void UpdateFromDTO(const DTO::WorldObject* dto) override
		{
			const DTO::Player* castedDto = static_cast<const DTO::Player*>(dto);
			WorldObject::UpdateFromDTO(dto);
			m_FullName = castedDto->fullName;
			m_Phenotype = castedDto->phenotype;
		}
		void SaveState() override
		{
			WorldObject::SaveState();
			m_PrevState =
			{
				m_FullName,
				m_Phenotype
			};
		}
		const static Player CreateFromDTO(const DTO::Player& dto)
		{
			return Player(
				dto.id,
				dto.transform,
				dto.fullName,
				dto.phenotype
			);
		}
		const bool IsEqual(const DTO::WorldObject* dto) const override
		{
			const DTO::Player* castedDto = static_cast<const DTO::Player*>(dto);
			return WorldObject::IsEqual(dto) &&
				m_FullName.IsEqual(&castedDto->fullName) &&
				m_Phenotype.IsEqual(&castedDto->phenotype);
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
		struct State
		{
			ValueObjects::FullName fullName = ValueObjects::FullName();
			ValueObjects::Phenotype phenotype = ValueObjects::Phenotype();

			bool isNewState = true;
		};

	private:
		ValueObjects::FullName m_FullName = ValueObjects::FullName();
		ValueObjects::Phenotype m_Phenotype = ValueObjects::Phenotype();
		State m_PrevState = State();
	};
}
