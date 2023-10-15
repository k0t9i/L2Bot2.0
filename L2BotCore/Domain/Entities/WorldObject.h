#pragma once
#include <cstdint>
#include <functional>
#include "../ValueObjects/Transform.h"
#include "EntityInterface.h"
#include "../Helpers/HashCombiner.h"

namespace L2Bot::Domain::Entities
{
	class WorldObject : public EntityInterface
	{
	public:
		virtual const uint32_t GetId() const override
		{
			return m_Id;
		}
		virtual void Update(const ValueObjects::Transform& transform)
		{
			m_Transform = transform;
		}
		virtual const size_t GetHash() const override
		{
			return Helpers::CombineHashes({
				std::hash<uint32_t>{}(m_Id),
				m_Transform.GetHash()
			});
		}

		virtual const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			result.push_back({ L"id", std::to_wstring(GetId()) });
			result.push_back({ L"transform", m_Transform.BuildSerializationNodes() });

			return result;
		}

		WorldObject(const uint32_t id, const ValueObjects::Transform& transform) :
			m_Id(id), m_Transform(transform)
		{

		}

		WorldObject() = default;
		virtual ~WorldObject() = default;

	private:
		uint32_t m_Id = 0;
		ValueObjects::Transform m_Transform = ValueObjects::Transform();
	};
}
