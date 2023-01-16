#pragma once
#include <vector>
#include "Node.h"

namespace L2Bot::Domain::Serializers
{
	class Serializable
	{
	public:
		virtual const std::vector<Node> BuildSerializationNodes() const = 0;
	};
}
