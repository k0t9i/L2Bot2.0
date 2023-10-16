#pragma once

#include <memory>
#include "../Events/EventDispatcher.h"

class ServiceLocator
{
public:
	static ServiceLocator& GetInstance() {
		static ServiceLocator instance;
		return instance;
	}

	const std::unique_ptr<EventDispatcher>& GetEventDispatcher()
	{
		return m_EventDispatcher;
	}
	void SetEventDispatcher(std::unique_ptr<EventDispatcher> dispatcher)
	{
		m_EventDispatcher = std::move(dispatcher);
	}
private:
	ServiceLocator() = default;
	virtual ~ServiceLocator() = default;

	ServiceLocator(const ServiceLocator&) = delete;
	ServiceLocator& operator=(const ServiceLocator&) = delete;
private:
	std::unique_ptr<EventDispatcher> m_EventDispatcher;
};