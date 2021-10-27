#include <thread>

#include "update.h"

void Update::Start(System& system)
{
    while(!system.Done())
    {
        system.Cpu().UpdateUtilization();
        system.UpdateMemoryUtilization();
        system.UpdateUpTime();
        system.UpdateProcesses();

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


