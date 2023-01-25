#pragma once
#include <string>
#include <vector>
#include "Serializable.h"

namespace L2Bot::Domain::Serializers
{
	class SerializerInterface
	{
	public:
		virtual const std::wstring Serialize(std::vector<Node> nodes, const bool isArray = false) const = 0;
	};
}
