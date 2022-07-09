#pragma once

#include <iostream>
#include <chrono>
	
/*
* Credits to The Cherno for this class
 */
class Timer
{
public:
	Timer();
	~Timer();
	void Stop();
private:
	std::chrono::time_point< std::chrono::high_resolution_clock> m_Start;
	std::chrono::time_point< std::chrono::high_resolution_clock> m_End;
};