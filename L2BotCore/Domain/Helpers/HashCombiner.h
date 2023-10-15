#pragma once

#include <vector>

namespace L2Bot::Domain::Helpers
{
	const size_t CombineHashes(const std::vector<size_t> hashes, size_t seed = 0);
}

