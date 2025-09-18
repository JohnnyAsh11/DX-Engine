#include <iostream>

#include "Stopwatch.h"
#include "ThreadManager.h"

using namespace std;

int complexFunction()
{
    // Pretending the sleep is a particular amount of time.
    std::this_thread::sleep_for(chrono::seconds(2));
    return 1;
}

void PerformTimeDiagnostic(void(*a_pFunction)())
{
    Stopwatch stopwatch = Stopwatch();
    stopwatch.Start();
    a_pFunction();
    stopwatch.Stop();
    cout << "Execution time: " << stopwatch.Result().count() << " seconds" << endl << endl;
}

int main()
{
    Stopwatch stopwatch = Stopwatch();

    PerformTimeDiagnostic([]() -> void {
            ThreadManager<int> threadMgr = ThreadManager<int>();
            for (int i = 0; i < 10; i++)
            {
                threadMgr.LaunchProcessAsync([]() -> int
                    {
                        return complexFunction();
                    });
            }
            threadMgr.AwaitAllTasks();
        });

    PerformTimeDiagnostic([]() -> void {
            for (int i = 0; i < 10; i++)
            {
                complexFunction();
            }
        });
}
