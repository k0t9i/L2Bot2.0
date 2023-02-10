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
		const std::shared_ptr<Entities::EntityInterface>& GetEntity() const
		{
			return m_Entity;
		}
		const Enums::EntityStateEnum GetState() const
		{
			return m_State;
		}
		void UpdateState(const Enums::EntityStateEnum state)
		{
			m_State = state;
		}

		EntityState(std::shared_ptr<Entities::EntityInterface> object, Enums::EntityStateEnum state) :
			m_Entity(object),
			m_State(state)
		{

		}

		EntityState() = default;
		virtual ~EntityState() = default;
	private:
		std::shared_ptr<Entities::EntityInterface> m_Entity = nullptr;
		Enums::EntityStateEnum m_State = Enums::EntityStateEnum::none;
	};
}