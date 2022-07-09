#include "Timer.h"

Timer::Timer()
{
	m_Start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	Stop();
}

void Timer::Stop()
{
	m_End = std::chrono::high_resolution_clock::now();
	
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_End).time_since_epoch().count();

	auto durationMicroSeconds = end - start;
	double millis = durationMicroSeconds * 0.001;

	std::cout << durationMicroSeconds << "us (" << millis << "ms)\n";
}
