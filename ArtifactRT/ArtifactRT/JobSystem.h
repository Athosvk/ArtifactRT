#pragma once
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <string>

#include "Random.h"

#include <Windows.h>

class JobThread {
private:
	//bool m_Waiting = true;
};

template<typename TJob>
class JobSystem
{
public:
	JobSystem() {
		size_t numThreads = std::thread::hardware_concurrency() - 1;
		m_Threads.reserve(numThreads);
		for (int i = 0; i < numThreads; i++)
		{
			m_Threads.emplace_back(std::thread([this]() {
					Random rng;
					while (true) {
						std::optional<TJob> job = Get();
						if (job != std::nullopt) {
							(*job)(rng);
						}
					}
				}
			));

#if defined(_WIN32)
			std::wstring name = L"Worker " + std::to_wstring(i);
			SetThreadDescription(m_Threads[i].native_handle(), name.c_str());
#endif
			
		}
	}

	void Spawn(TJob job) {
		std::scoped_lock jobsLock{ m_JobsMutex };
		m_Jobs.emplace_back(std::move(job));
	}

private:
	std::optional<TJob> Get() {
		std::unique_lock jobsLock{ m_JobsMutex };
		if (!m_Jobs.empty())
		{
			TJob job = std::move(m_Jobs.back());
			m_Jobs.pop_back();
			return job;
		}
		return {};
	}

	std::vector<std::thread> m_Threads;
	std::deque<TJob> m_Jobs;
	std::mutex m_JobsMutex;
	std::condition_variable m_NewJobNotify;

	std::atomic_bool m_Exit;
	std::condition_variable m_ExitNotify;
};

