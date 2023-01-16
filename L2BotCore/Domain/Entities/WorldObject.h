#pragma once
#include <cstdint>
#include "../ValueObjects/Transform.h"
#include "../DTO/WorldObject.h"
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::Entities
{
	class WorldObject : public Serializers::Serializable
	{
	public:
		const uint32_t GetId() const
		{
			return m_Id;
		}
		const ValueObjects::Transform& GetTransform() const
		{
			return m_Transform;
		}
		virtual void UpdateFromDTO(const DTO::WorldObject* dto)
		{
			SaveState();

			m_Id = dto->id;
			m_Transform = dto->transform;
		}
		virtual void SaveState()
		{
			m_PrevState = { m_Transform, false };
		}
		virtual const bool IsEqual(const DTO::WorldObject* dto) const
		{
			return m_Id == dto->id && m_Transform.IsEqual(&dto->transform);
		}
		const float_t GetSqrDistance(const WorldObject& other) const
		{
			return m_Transform.GetSqrDistance(other.m_Transform);
		}
		const float_t GetHorizontalSqrDistance(const WorldObject& other) const
		{
			return m_Transform.GetHorizontalSqrDistance(other.m_Transform);
		}

		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ "id", std::to_string(GetId()) });
			if (m_PrevState.isNewState || !GetTransform().IsEqual(&m_PrevState.transform))
			{
				result.push_back({ "transform", GetTransform().BuildSerializationNodes() });
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
		struct State
		{
			ValueObjects::Transform transform = ValueObjects::Transform();

			bool isNewState = true;
		};

	private:
		uint32_t m_Id = 0;
		ValueObjects::Transform m_Transform = ValueObjects::Transform();
		State m_PrevState = State();
	};
}
