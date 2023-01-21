#pragma once
#include <cstdint>
#include "../ValueObjects/Transform.h"
#include "EntityInterface.h"

namespace L2Bot::Domain::Entities
{
	class WorldObject : public EntityInterface
	{
	public:
		virtual const uint32_t GetId() const override
		{
			return m_Id;
		}
		virtual void Update(const EntityInterface* other) override
		{
			SaveState();

			const WorldObject* casted = static_cast<const WorldObject*>(other);
			m_Id = casted->m_Id;
			m_Transform = casted->m_Transform;
		}
		virtual void SaveState() override
		{
			m_PrevState = { m_Transform, false };
		}
		virtual const bool IsEqual(const EntityInterface* other) const override
		{
			const WorldObject* casted = static_cast<const WorldObject*>(other);
			return m_Id == casted->m_Id && m_Transform.IsEqual(&casted->m_Transform);
		}

		virtual const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ "id", std::to_string(GetId()) });
			if (m_PrevState.isNewState || !m_Transform.IsEqual(&m_PrevState.transform))
			{
				result.push_back({ "transform", m_Transform.BuildSerializationNodes() });
			}

			return result;
		}

		WorldObject(const uint32_t id, const ValueObjects::Transform transform) :
			m_Id(id), m_Transform(transform)
		{

		}

		WorldObject() = default;
		virtual ~WorldObject() = default;
	private:
	private:
		struct GetState
		{
			ValueObjects::Transform transform = ValueObjects::Transform();

			bool isNewState = true;
		};

	private:
		uint32_t m_Id = 0;
		ValueObjects::Transform m_Transform = ValueObjects::Transform();
		GetState m_PrevState = GetState();
	};
}
