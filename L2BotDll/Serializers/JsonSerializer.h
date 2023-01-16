#pragma once

#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Serializers/Node.h"

using namespace L2Bot::Domain;

class JsonSerializer : public Serializers::SerializerInterface
{
public:
	const std::string Serialize(std::vector<Serializers::Node> nodes, const bool isArray = false) const override
	{
		std::string result = isArray ? "[" : "{";

		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			if (!isArray)
			{
				result += "\"" + it->name + "\":";
			}
			if (it->isContainer)
			{
				result += Serialize(it->children, it->isArray);
			}
			else
			{
				result += "\"" + it->value + "\"";
			}
			if (std::next(it) != nodes.end())
			{
				result += ",";
			}
		}

		result += isArray ? "]" : "}";

		return result;
	}
};