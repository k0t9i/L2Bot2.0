#pragma once

#include <memory>
#include "../Events/EventDispatcher.h"

namespace L2Bot::Domain::Services
{
	class ServiceLocator
	{
	public:
		static ServiceLocator& GetInstance() {
			static ServiceLocator instance;
			return instance;
		}

		const std::unique_ptr<Events::EventDispatcher>& GetEventDispatcher()
		{
			return m_EventDispatcher;
		}
		void SetEventDispatcher(std::unique_ptr<Events::EventDispatcher> dispatcher)
		{
			m_EventDispatcher = std::move(dispatcher);
		}
	private:
		ServiceLocator() = default;
		virtual ~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
	private:
		std::unique_ptr<Events::EventDispatcher> m_EventDispatcher;
	};
}