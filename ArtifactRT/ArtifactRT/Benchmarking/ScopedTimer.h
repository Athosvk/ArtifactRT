#pragma once
#include <chrono>

class ScopedTimer
{
public:
	ScopedTimer();

	long long GetDurationNanoseconds() const;
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
};

