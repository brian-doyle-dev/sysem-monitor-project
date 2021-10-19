#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <thread>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


/**
 * @brief Return the CPU info
 * @return The CPU info
 */
Processor& System::Cpu() { 
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

/**
 * @brief Return kernel version 
 * @return The kernel version
 */
std::string System::Kernel() { 
   return LinuxParser::Attribute<std::string>(std::string(LinuxParser::KernelRegex), 
                                              LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
 }

/**
 * @brief Update the memory utilization by reading the system files.
 * @return (void)
 */
void System::UpdateMemoryUtilization() { 
  int total = LinuxParser::Attribute<int>(LinuxParser::MemTotalRegex, LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  int free = LinuxParser::Attribute<int>(LinuxParser::MemFreeRegex, LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  
  memoryUtilization = (float(total - free) / float(total)) * 100.0;
}

/**
 * @brief Return the memory utilization
 * @return The memory utilization
 */
float System::MemoryUtilization() { 
    return memoryUtilization;
}

/**
 * @brief Return operating system name
 * @return The operating system name
 */
std::string System::OperatingSystem() { 
  return LinuxParser::Attribute<std::string>(std::string(LinuxParser::OsRegex), 
                                              LinuxParser::kOSPath);

}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

/**
 * @brief Update the system uptime by reading the system file
 * @return (void)
 */
void System::UpdateUpTime() { 
  uptime = LinuxParser::Attribute<long>(LinuxParser::UptimeRegex, LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
}

/**
 * @brief Return the system uptime
 * @return The system uptime
 */
long System::UpTime() { 
  return uptime;
}