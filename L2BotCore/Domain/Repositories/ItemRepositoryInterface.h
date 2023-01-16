#pragma once
#include <cstdint>
#include <map>
#include "ObjectRepositoryInterface.h"
#include "../DTO/BaseItem.h"

namespace L2Bot::Domain::Repositories
{
	class ItemRepositoryInterface : public ObjectRepositoryInterface<DTO::BaseItem>
	{
	public:
		virtual const std::map<uint32_t, DTO::BaseItem> GetObjects() override = 0;
	};
}
