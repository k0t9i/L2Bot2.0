#pragma once

#include "Domain/Serializers/SerializerInterface.h"
#include "Domain/Serializers/Node.h"

using namespace L2Bot::Domain;

class JsonSerializer : public Serializers::SerializerInterface
{
public:
	const std::wstring Serialize(std::vector<Serializers::Node> nodes, const bool isArray = false) const override
	{
		std::wstring result = isArray ? L"[" : L"{";

		for (auto it = nodes.begin(); it != nodes.end(); ++it)
		{
			if (!isArray)
			{
				result += L"\"" + it->name + L"\":";
			}
			if (it->isContainer)
			{
				result += Serialize(it->children, it->isArray);
			}
			else
			{
				result += L"\"" + it->value + L"\"";
			}
			if (std::next(it) != nodes.end())
			{
				result += L",";
			}
		}

		result += isArray ? L"]" : L"}";

		return result;
	}
};