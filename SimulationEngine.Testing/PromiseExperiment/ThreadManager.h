#ifndef __THREADMANAGER_H_
#define __THREADMANAGER_H_

#include <thread>
#include <future>
#include <queue>

/// <summary>
/// Manages the processes executing on other threads.
/// </summary>
/// <typeparam name="T">The return type of functions managed by this class.</typeparam>
template <class T>
class ThreadManager
{
private:
	std::vector<std::future<T>> m_lTasks;
	std::queue<T> m_lResults;
	
public:
	/// <summary>
	/// Enqueues a certain function as a task to be completed asynchronously.
	/// </summary>
	/// <param name="a_pFunction">The function being completed asynchronously.</param>
	void LaunchProcessAsync(T(*a_pFunction)())
	{
		// Sending out the function to be completed parallel to the main thread.
		std::future<T> task = std::async(std::launch::async, a_pFunction);

		m_lTasks.push_back(std::move(task));

		// Adding the future to the collection of tasks.
		//return task.get();
	}

	void AwaitAllTasks()
	{
		for (int i = 0; i < m_lTasks.size(); i++)
		{
			m_lTasks[i].get();
		}
	}

	/// <summary>
	/// Awaits the oldest process and returns its result.
	/// </summary>
	/// <returns>The value from the completed task.</returns>
	/*T AwaitOldestProcess()
	{
		std::future<T> task = m_lTasks.front();
		m_lTasks.pop();
		T result = task.get();

		return result;
	}*/
};

#endif //__THREADMANAGER_H_

