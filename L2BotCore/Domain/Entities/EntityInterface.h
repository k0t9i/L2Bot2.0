#pragma once

#include <cstdint>
#include <memory>
#include "../Serializers/Serializable.h"

namespace L2Bot::Domain::Entities
{
	class EntityInterface : public Serializers::Serializable
	{
	public:
		virtual const uint32_t GetId() const = 0;
		virtual void Update(const EntityInterface* other) = 0;
		virtual void SaveState() = 0;
		virtual const bool IsEqual(const EntityInterface* other) const = 0;

		EntityInterface() = default;
		virtual ~EntityInterface() = default;
	};
}
