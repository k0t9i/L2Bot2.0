#pragma once

#include <memory>
#include "../Events/EventDispatcher.h"
#include "../Logger/Logger.h"

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

		const std::unique_ptr<Logger::Logger>& GetLogger()
		{
			return m_Logger;
		}
		void SetLogger(std::unique_ptr<Logger::Logger> logger)
		{
			m_Logger = std::move(logger);
		}
	private:
		ServiceLocator() = default;
		virtual ~ServiceLocator() = default;

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
	private:
		std::unique_ptr<Events::EventDispatcher> m_EventDispatcher;
		std::unique_ptr<Logger::Logger> m_Logger;
	};
}