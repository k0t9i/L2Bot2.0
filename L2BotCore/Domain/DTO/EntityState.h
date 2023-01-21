#pragma once

#include "../Entities/EntityInterface.h"
#include "../Enums/EntityStateEnum.h"

namespace L2Bot::Domain::DTO
{
	class EntityState
	{
	public:
		Entities::EntityInterface* GetEntity() const
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

		EntityState(Entities::EntityInterface* object, Enums::EntityStateEnum state) :
			m_Entity(object),
			m_State(state)
		{

		}

		EntityState() = default;
		virtual ~EntityState()
		{
			if (m_Entity != nullptr)
			{
				delete m_Entity;
			}
		}
	private:
		Entities::EntityInterface* m_Entity = nullptr;
		Enums::EntityStateEnum m_State = Enums::EntityStateEnum::none;
	};
}