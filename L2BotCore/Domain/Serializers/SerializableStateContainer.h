#pragma once
#include <vector>
#include "../DTO/EntityState.h"
#include "Serializable.h"

namespace L2Bot::Domain::Serializers
{
	class SerializableStateContainer : public Serializers::Serializable
	{
	public:
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			for (const auto& kvp : m_Objects)
			{
				std::wstring operationName = L"";
				switch (kvp->GetState())
				{
				case Enums::EntityStateEnum::created:
					operationName = L"created";
					break;
				case Enums::EntityStateEnum::updated:
					operationName = L"updated";
					break;
				case Enums::EntityStateEnum::deleted:
					operationName = L"deleted";
					break;
				}

				if (operationName != L"")
				{
					result.push_back(
						{ 
							m_ContainerName,
							std::vector<Serializers::Node>{ { operationName, kvp->GetEntity()->BuildSerializationNodes() } }
						}
					);
				}
			}

			return result;
		}

		SerializableStateContainer(const std::vector<std::shared_ptr<DTO::EntityState>> objects, const std::wstring& containerName) :
			m_Objects(objects), m_ContainerName(containerName)
		{

		}
		SerializableStateContainer() = delete;
		virtual ~SerializableStateContainer() = default;
	private:
		const std::vector<std::shared_ptr<DTO::EntityState>> m_Objects;
		const std::wstring m_ContainerName;
	};
}