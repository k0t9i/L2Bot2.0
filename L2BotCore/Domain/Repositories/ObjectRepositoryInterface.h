#pragma once
#include <cstdint>
#include <map>

namespace L2Bot::Domain::Repositories
{
	template <typename T>
	class ObjectRepositoryInterface
	{
	public:
		virtual const std::map<uint32_t, T> GetObjects() = 0;
	};
}
