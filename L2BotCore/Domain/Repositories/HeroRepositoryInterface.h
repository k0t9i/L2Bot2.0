#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/Hero.h"

namespace L2Bot::Domain::Repositories
{
	class HeroRepositoryInterface : public ObjectRepositoryInterface<DTO::Hero>
	{
	public:
		virtual const std::map<uint32_t, DTO::Hero> GetObjects() override = 0;
	};
}
