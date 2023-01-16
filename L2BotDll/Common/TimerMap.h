#pragma once

#include <map>
#include <cstdint>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <chrono>

class TimerMap
{
public:
	TimerMap() = default;
	virtual ~TimerMap()
	{
		StopAll();
	}

	void StartTimer(const uint32_t key, const uint32_t milliseconds, const std::function<void(uint32_t)> callback)
	{
		StopTimer(key);

		m_Timers[key].Start(milliseconds, callback, key);
	}

	void StopTimer(uint32_t key)
	{
		if (m_Timers.find(key) != m_Timers.end())
		{
			m_Timers[key].Stop();
		}
	}

	void StopAll()
	{
		m_Timers.clear();
	}

private:
	class Timer
	{
	public:
		void Start(const uint32_t milliseconds, const std::function<void(uint32_t)> callback, const uint32_t data)
		{
			m_Terminate = false;
			m_Thread = std::thread([this, milliseconds, callback, data] {
				std::unique_lock<std::mutex> lk(m_Mutex);

				if (!m_Condition.wait_for(lk, std::chrono::milliseconds(milliseconds), [this]() { return m_Terminate == true; }))
				{
					callback(data);
				}
			});
		}

		void Stop()
		{
			m_Terminate = true;
			m_Condition.notify_all();
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}

		Timer() = default;
		virtual ~Timer()
		{
			Stop();

		}
	private:
		std::condition_variable m_Condition;
		std::mutex m_Mutex;
		std::atomic_bool m_Terminate = false;
		std::thread m_Thread;
	};

	std::map<uint32_t, Timer> m_Timers;
};