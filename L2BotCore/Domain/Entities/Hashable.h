#pragma once

#include <string>

namespace L2Bot::Domain::Entities
{
	class Hashable
	{
	public:
		virtual const std::size_t GetHash() const = 0;

		Hashable() = default;
		virtual ~Hashable() = default;
	};
}