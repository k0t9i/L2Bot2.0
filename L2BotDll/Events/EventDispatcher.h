#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include "Event.h"

class EventDispatcher
{
public:
	using Delegate = std::function<void(const Event&)>;

	EventDispatcher() = default;
	virtual ~EventDispatcher() = default;

	void Dispatch(const Event& evt)
	{
		const auto& name = evt.GetName();

		if (m_Handlers.find(name) == m_Handlers.end())
		{
			return;
		}

		for (const auto& handler : m_Handlers[name])
		{
			handler(evt);
		}
	}

	void Subscribe(const std::string& eventName, const Delegate handler)
	{
		m_Handlers[eventName].push_back(handler);
	}

private:
	std::unordered_map<std::string, std::vector<Delegate>> m_Handlers;
};