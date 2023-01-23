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
				std::string operationName = "";
				switch (kvp->GetState())
				{
				case Enums::EntityStateEnum::created:
					operationName = "created";
					break;
				case Enums::EntityStateEnum::updated:
					operationName = "updated";
					break;
				case Enums::EntityStateEnum::deleted:
					operationName = "deleted";
					break;
				}

				if (operationName != "")
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

		SerializableStateContainer(const std::vector<std::shared_ptr<DTO::EntityState>> objects, const std::string containerName) :
			m_Objects(objects), m_ContainerName(containerName)
		{

		}
		SerializableStateContainer() = delete;
		virtual ~SerializableStateContainer() = default;
	private:
		const std::vector<std::shared_ptr<DTO::EntityState>> m_Objects;
		const std::string m_ContainerName;
	};
}