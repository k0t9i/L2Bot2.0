#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include <math.h>
#include "../DTO/ObjectState.h"
#include "../Entities/Hero.h"
#include "../Repositories/ObjectRepositoryInterface.h"

namespace L2Bot::Domain::Services
{
	template <typename T, typename U>
	class ObjectServicePtr
	{
	public:
		ObjectServicePtr(Repositories::ObjectRepositoryInterface<U>& repository) : m_Repository(repository)
		{

		}ObjectServicePtr() = delete;
		virtual ~ObjectServicePtr() = default;

		virtual const std::vector<DTO::ObjectState<T>> GetObjects()
		{
			UpdateObjectsFromRepository();

			std::vector<DTO::ObjectState<T>> objects;

			for (const auto& kvp : m_ObjectStates) {
				objects.push_back(kvp.second);
			}

			return objects;
		}

		void Invalidate()
		{
			m_ObjectStates.clear();
		}

	protected:
		virtual void UpdateObjectsFromRepository()
		{
			auto objects = m_Repository.GetObjects();

			RemoveOutdatedStates();


			for (const auto& kvp : objects)
			{
				const auto& dto = kvp.second;
				if (m_ObjectStates.contains(kvp.first))
				{
					if (!m_ObjectStates[kvp.first].object->IsEqual(dto.get())) {
						m_ObjectStates[kvp.first].object->UpdateFromDTO(dto.get());
						m_ObjectStates[kvp.first].state = Enums::ObjectStateEnum::updated;
					}
					else
					{
						m_ObjectStates[kvp.first].state = Enums::ObjectStateEnum::none;
					}
				}
				else
				{
					//m_ObjectStates.emplace(kvp.first, DTO::ObjectState<T>{ T::CreateFromDTO(dto), Enums::ObjectStateEnum::created });
				}
			}

			for (auto& kvp : m_ObjectStates)
			{
				if (!objects.contains(kvp.second.object->GetId()))
				{
					m_ObjectStates[kvp.first].object->SaveState();
					kvp.second.state = Enums::ObjectStateEnum::deleted;
				}
			}
		}

	private:
		void RemoveOutdatedStates()
		{
			auto it = m_ObjectStates.begin();
			while (it != m_ObjectStates.end())
			{
				if (it->second.state == Enums::ObjectStateEnum::deleted)
				{
					m_ObjectStates.erase(it++);
				}
				else
				{
					it++;
				}
			}
		}

	private:
		Repositories::ObjectRepositoryInterface<U>& m_Repository;
		std::map<uint32_t, DTO::ObjectState<T>> m_ObjectStates;
	};
}
