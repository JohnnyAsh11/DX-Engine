#ifndef __STOPWATCH_H_
#define __STOPWATCH_H_

#include <chrono>

typedef std::chrono::steady_clock::time_point Time;
typedef std::chrono::duration<double> DeltaTime;

/// <summary>
/// Performs time diagnostics for processes in a program.
/// </summary>
class Stopwatch
{
private:
	DeltaTime m_result;
	Time m_start;
	Time m_end;

public:
	/// <summary>
	/// Saves the start time.
	/// </summary>
	void Start();

	/// <summary>
	/// Saves the end time.
	/// </summary>
	void Stop();

	/// <summary>
	/// Performs the necessary calculation to 
	/// get the amount of time spent between 
	/// the Start and Stop method calls.
	/// </summary>
	DeltaTime Result();
};

#endif //__STOPWATCH_H_
