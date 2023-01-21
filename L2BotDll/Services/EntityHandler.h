#pragma once

#include <map>
#include <functional>
#include "Domain/Repositories/EntityRepositoryInterface.h"
#include "Domain/DTO/EntityState.h"

using namespace L2Bot::Domain;

class EntityHandler
{
public:
	template<typename T>
	const std::map<uint32_t, DTO::EntityState*> GetEntities(const std::map<uint32_t, T> items, std::function<Entities::EntityInterface*(T)> callback)
	{
		RemoveOutdatedStates();

		for (const auto& kvp : items)
		{
			const auto item = kvp.second;
			const auto newObject = callback(item);
			if (m_Objects.contains(newObject->GetId()))
			{
				if (!m_Objects[kvp.first]->GetEntity()->IsEqual(newObject)) {
					m_Objects[kvp.first]->GetEntity()->Update(newObject);
					m_Objects[kvp.first]->UpdateState(Enums::EntityStateEnum::updated);
				}
				else
				{
					m_Objects[kvp.first]->UpdateState(Enums::EntityStateEnum::none);
				}
				delete newObject;
			}
			else
			{
				m_Objects.emplace(
					newObject->GetId(),
					new DTO::EntityState{ newObject, Enums::EntityStateEnum::created }
				);
			}
		}

		for (auto& kvp : m_Objects)
		{
			if (!items.contains(kvp.second->GetEntity()->GetId()))
			{
				m_Objects[kvp.first]->GetEntity()->SaveState();
				kvp.second->UpdateState(Enums::EntityStateEnum::deleted);
			}
		}

		return m_Objects;
	}

	void Reset()
	{
		for (const auto& object : m_Objects)
		{
			delete object.second;
		}
		m_Objects.clear();
	}

	EntityHandler() = default;
	virtual ~EntityHandler()
	{
		Reset();
	}
private:
	void RemoveOutdatedStates()
	{
		auto it = m_Objects.begin();
		while (it != m_Objects.end())
		{
			if (it->second->GetState() == Enums::EntityStateEnum::deleted)
			{
				delete it->second;
				m_Objects.erase(it++);
			}
			else
			{
				it++;
			}
		}
	}

private:
	std::map<uint32_t, DTO::EntityState*> m_Objects;
};