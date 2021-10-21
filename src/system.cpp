#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
{
  System::UpdateProcesses();
}

/**
 * @brief Return the CPU info
 * @return The CPU info
 */
Processor& System::Cpu() { 
  return cpu_; 
}

void System::AddNewProcess(int pid)
{
  Process newProcess {pid};
  processes_.push_back(newProcess);
}

void System::RemoveTerminatedProcesses()
{

}

void System::UpdateProcesses()
{
  std::vector<int> pids = LinuxParser::Pids();

  for(auto pid : pids)
  {
    if (processes_.size() == 0)
    {
      AddNewProcess(pid);
    }
    else
    {
      bool duplicate = false;
      for(auto process : processes_)
      {
        if (process.Pid() == pid)
        {
          duplicate = true;
        }
      }

      if (!duplicate)
      {
        AddNewProcess(pid);
      }
    }
  }

  for (auto& process : processes_)
  {
    process.UpdateProcess();

    //std::cout << process.Command();
  }
//  sort(processes_.begin(), processes_.end(), Process::Compare);
  sort(processes_.begin(), processes_.end());
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  return processes_;
}

/**
 * @brief Return kernel version 
 * @return The kernel version
 */
std::string System::Kernel() { 
  return LinuxParser::Attribute<std::string>(LinuxParser::KernelRegex, LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
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
  return LinuxParser::Attribute<std::string>(LinuxParser::OsRegex, LinuxParser::kOSPath);

}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {

  return 0;

}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return processes_.size();
}

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