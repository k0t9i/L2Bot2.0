#pragma once
#include <vector>
#include "../DTO/ObjectState.h"
#include "Serializable.h"

namespace L2Bot::Domain::Serializers
{
	template <typename T>
	class SerializableStateContainerPtr : public Serializers::Serializable
	{
	public:
		const std::vector<Serializers::Node> BuildSerializationNodes() const override
		{
			std::vector<Serializers::Node> result;

			for (const auto& kvp : m_Objects)
			{
				std::string operationName = "";
				switch (kvp.state)
				{
				case Enums::ObjectStateEnum::created:
					operationName = "created";
					break;
				case Enums::ObjectStateEnum::updated:
					operationName = "updated";
					break;
				case Enums::ObjectStateEnum::deleted:
					operationName = "deleted";
					break;
				}

				if (operationName != "")
				{
					result.push_back(
						{
							m_ContainerName,
							std::vector<Serializers::Node>{ { operationName, kvp.object->BuildSerializationNodes() } }
						}
					);
				}
			}

			return result;
		}

		SerializableStateContainerPtr(const std::vector<DTO::ObjectState<T>> objects, const std::string containerName) :
			m_Objects(objects), m_ContainerName(containerName)
		{

		}
		SerializableStateContainerPtr() = delete;
		virtual ~SerializableStateContainerPtr() = default;
	private:
		const std::vector<DTO::ObjectState<T>> m_Objects;
		const std::string m_ContainerName;
	};
}