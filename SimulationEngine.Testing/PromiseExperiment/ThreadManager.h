#ifndef __THREADMANAGER_H_
#define __THREADMANAGER_H_

#include <thread>
#include <future>

/// <summary>
/// Manages the processes executing on other threads.
/// </summary>
/// <typeparam name="T">The return type of functions managed by this class.</typeparam>
class ThreadManager
{
private:
	std::vector<std::future<void>> m_lTasks;
	
public:
	/// <summary>
	/// Enqueues a certain function as a task to be completed asynchronously on a different thread.
	/// </summary>
	/// <param name="a_pFunction">The function being completed asynchronously.</param>
	void LaunchProcessAsync(void(*a_pFunction)())
	{
		// Sending out the function to be completed parallel to the main thread.
		std::future<void> task = std::async(std::launch::async, a_pFunction);

		// Moving the future to the collection of futures.
		m_lTasks.push_back(std::move(task));
	}

	/// <summary>
	/// Waits for the results of all futures in the task collection.
	/// </summary>
	void AwaitAllTasks()
	{
		// Awaiting all futures to be finished.
		for (int i = 0; i < m_lTasks.size(); i++)
		{
			m_lTasks[i].get();
		}

		// Emptying the collection of futures.
		m_lTasks.clear();
	}
};

#endif //__THREADMANAGER_H_

