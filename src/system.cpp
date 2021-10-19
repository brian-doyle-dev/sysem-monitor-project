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

// TODO: Return the system's kernel identifier (string)
/**
 * @brief Return kernel version 
 * @return The kernel version
 */
std::string System::Kernel() { 
   return LinuxParser::Attribute<std::string>(std::string(LinuxParser::KernelRegex), 
                                              LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
 }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

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

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }