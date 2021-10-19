#include <thread>

#include "update.h"
#include "system.h"

void Update::Start(System& system)
{
    while(1)
    {
        system.Cpu().UpdateUtilization();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


