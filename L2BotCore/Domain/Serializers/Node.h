#pragma once
#include <map>
#include <vector>
#include <string>

namespace L2Bot::Domain::Serializers
{
	struct Node
	{
		const std::wstring name = L"";
		const std::wstring value = L"";
		const std::vector<Node> children;
		const bool isArray = false;
		const bool isContainer = false;

		Node() = delete;
		Node(const std::wstring& name, const std::wstring& value) :
			name(name), value(value)
		{
		}
		Node(const std::wstring& name, const std::vector<Node> children, const bool isArray = false) :
			name(name), children(children), isArray(isArray), isContainer(true)
		{
		}
	};
}
