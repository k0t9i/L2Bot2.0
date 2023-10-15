#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "../Serializers/Serializable.h"
#include "Hashable.h"

namespace L2Bot::Domain::Entities
{
	class EntityInterface : public Serializers::Serializable, public Hashable
	{
	public:
		virtual const uint32_t GetId() const = 0;
		virtual const std::string GetEntityName() const = 0;

		EntityInterface() = default;
		virtual ~EntityInterface() = default;
	};
}
