#pragma once

#include <cstdint>
#include <memory>
#include "../Entities/EntityInterface.h"
#include "../Enums/EntityStateEnum.h"

namespace L2Bot::Domain::DTO
{
	class EntityState
	{
	public:
		const uint32_t GetId() const
		{
			return m_Entity->GetId();
		}
		const std::vector<Serializers::Node> BuildSerializationNodes() const
		{
			return m_Entity->BuildSerializationNodes();
		}
		void SaveEntityState()
		{
			m_Entity->SaveState();
		}
		const bool IsEntityEqual(const Entities::EntityInterface* other) const
		{
			return m_Entity->IsEqual(other);
		}
		void UpdateEntity(const Entities::EntityInterface* other)
		{
			m_Entity->Update(other);
		}

		const Enums::EntityStateEnum GetState() const
		{
			return m_State;
		}
		void UpdateState(const Enums::EntityStateEnum state)
		{
			m_State = state;
		}

		EntityState(std::unique_ptr<Entities::EntityInterface> object, Enums::EntityStateEnum state) :
			m_Entity(std::move(object)),
			m_State(state)
		{

		}

		EntityState() = default;
		virtual ~EntityState() = default;
	private:
		std::unique_ptr<Entities::EntityInterface> m_Entity = nullptr;
		Enums::EntityStateEnum m_State = Enums::EntityStateEnum::none;
	};
}