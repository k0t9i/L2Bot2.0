#pragma once
#include <cstdint>
#include <map>
#include "ObjectService.h"
#include "../DTO/BaseItem.h"
#include "../ValueObjects/BaseItem.h"
#include "../Repositories/ItemRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	class ItemService : public ObjectService<ValueObjects::BaseItem, DTO::BaseItem>
	{
	public:
		ItemService(Repositories::ItemRepositoryInterface& repository) : ObjectService(repository)
		{

		}
		virtual ~ItemService() override = default;
	};
}
