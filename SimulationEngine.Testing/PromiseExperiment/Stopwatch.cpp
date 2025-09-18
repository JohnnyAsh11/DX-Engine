#include "Stopwatch.h"

void Stopwatch::Start()
{
	m_start = std::chrono::high_resolution_clock::now();
}

void Stopwatch::Stop()
{
	m_end = std::chrono::high_resolution_clock::now();
}

DeltaTime Stopwatch::Result()
{
	m_result = m_end - m_start;
	return m_result;
}