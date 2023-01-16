#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/Hero.h"
#include "../Entities/Hero.h"
#include "../Repositories/HeroRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class HeroService : public ObjectService<Entities::Hero, DTO::Hero>
	{
	public:
		const DTO::ObjectState<Entities::Hero> GetHero()
		{
			const auto map = GetObjects();
			if (map.size() == 0)
			{
				return DTO::ObjectState <Entities::Hero>{};
			}

			return map[0];
		}

		HeroService(Repositories::HeroRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~HeroService() override = default;
	};
}
