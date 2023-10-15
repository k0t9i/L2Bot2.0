#include "..\..\pch.h"
#include "HashCombiner.h"

namespace L2Bot::Domain::Helpers
{
	const size_t CombineHashes(const std::vector<size_t> hashes, size_t seed)
	{
		for (const auto hash : hashes) {
			seed = hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed;
	}
}