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

	static EventDispatcher& GetInstance() {
		static EventDispatcher instance;
		return instance;
	}

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

	void Subscribe(std::string eventName, Delegate handler)
	{
		m_Handlers[eventName].push_back(handler);
	}

private:
	EventDispatcher() = default;
	virtual ~EventDispatcher() = default;
	EventDispatcher(const EventDispatcher&) = delete;
	EventDispatcher& operator=(const EventDispatcher&) = delete;

private:
	std::unordered_map<std::string, std::vector<Delegate>> m_Handlers;
};