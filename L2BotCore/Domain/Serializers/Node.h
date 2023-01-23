#pragma once
#include <map>
#include <vector>
#include <string>

namespace L2Bot::Domain::Serializers
{
	struct Node
	{
		const std::string name = "";
		const std::string value = "";
		const std::vector<Node> children;
		const bool isArray = false;
		const bool isContainer = false;

		Node() = delete;
		Node(const std::string& name, const std::string& value) :
			name(name), value(value)
		{
		}
		Node(const std::string& name, const std::vector<Node> children, const bool isArray = false) :
			name(name), children(children), isArray(isArray), isContainer(true)
		{
		}
	};
}
