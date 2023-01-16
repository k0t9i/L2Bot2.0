#pragma once

#include <map>
#include "Domain/Repositories/DropRepositoryInterface.h"
#include "../Factories/DropFactory.h"
#include "../../GameStructs/FindObjectsTrait.h"

using namespace L2Bot::Domain;

namespace Interlude
{
	class DropRepository : public Repositories::DropRepositoryInterface, public FindObjectsTrait
	{
	public:
		const std::map<uint32_t, DTO::Drop> GetObjects() override
		{
			const auto items = GetAllObjects<Item*>(m_Radius, [this](float_t radius, int32_t prevId) {
				return m_NetworkHandler.GetNextItem(radius, prevId);
			});

			std::map<uint32_t, DTO::Drop> map;

			for (const auto& kvp : items)
			{
				const auto item = kvp.second;
				map.emplace(item->objectId, m_Factory.Create(item));
			}

			return map;
		}

		DropRepository(const NetworkHandlerWrapper& networkHandler, const DropFactory& factory, const uint16_t radius) :
			m_NetworkHandler(networkHandler),
			m_Factory(factory),
			m_Radius(radius)
		{

		}

		DropRepository() = delete;
		virtual ~DropRepository() = default;

	private:
		const NetworkHandlerWrapper& m_NetworkHandler;
		const DropFactory& m_Factory;
		const uint16_t m_Radius;
	};
}
