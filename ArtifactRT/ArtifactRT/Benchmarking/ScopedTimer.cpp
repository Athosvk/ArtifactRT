#include "ScopedTimer.h"

ScopedTimer::ScopedTimer() : 
	m_StartTime(std::chrono::high_resolution_clock::now())
{
}

long long ScopedTimer::GetDurationNanoseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
}
