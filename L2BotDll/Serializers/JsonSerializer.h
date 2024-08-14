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
				result += L"\"" + EscapeJson(it->value) + L"\"";
			}
			if (std::next(it) != nodes.end())
			{
				result += L",";
			}
		}

		result += isArray ? L"]" : L"}";

		return result;
	}

private:
	const std::wstring EscapeJson(const std::wstring& input) const
	{
		std::wstringstream o;
		for (auto c = input.cbegin(); c != input.cend(); c++) {
			switch (*c) {
			case '"': o << "\\\""; break;
			case '\\': o << "\\\\"; break;
			case '\b': o << "\\b"; break;
			case '\f': o << "\\f"; break;
			case '\n': o << "\\n"; break;
			case '\r': o << "\\r"; break;
			case '\t': o << "\\t"; break;
			default:
				if ('\x00' <= *c && *c <= '\x1f') {
					o << "\\u"
						<< std::hex << std::setw(4) << std::setfill(L'0') << static_cast<int>(*c);
				}
				else {
					o << *c;
				}
			}
		}
		return o.str();
	}
};