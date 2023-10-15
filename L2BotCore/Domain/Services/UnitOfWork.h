#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include "../Entities/EntityInterface.h"
#include "../Enums/EntityStateEnum.h"

namespace L2Bot::Domain::Services
{
	class UnitOfWork
	{
	public:
		const std::unordered_map<uint32_t, Enums::EntityStateEnum> ConnectEntities(const std::wstring& name, const std::unordered_map<uint32_t, std::shared_ptr<Entities::EntityInterface>>& entites)
		{
			std::unordered_map<uint32_t, Enums::EntityStateEnum> result;

			auto& hashes = m_Hashes[name];

			for (const auto& kvp : entites) {
				const auto &entity = kvp.second;

				if (hashes.size() == 0) {
					result[entity->GetId()] = Enums::EntityStateEnum::created;
				}
				else
				{
					if (hashes.find(entity->GetId()) == hashes.end()) {
						result[entity->GetId()] = Enums::EntityStateEnum::created;
					}
					else if (hashes[entity->GetId()] != entity->GetHash()) {
						result[entity->GetId()] = Enums::EntityStateEnum::updated;
					}
				}

				hashes[entity->GetId()] = entity->GetHash();
			}


			for (auto it = hashes.begin(); it != hashes.end();)
			{
				if (entites.find(it->first) == entites.end())
				{
					result[it->first] = Enums::EntityStateEnum::deleted;
					it = hashes.erase(it);
				}
				else
				{
					++it;
				}
			}

			return result;
		}

	private:
		std::unordered_map <std::wstring, std::unordered_map<uint32_t, size_t>> m_Hashes;
	};
}