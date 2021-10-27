#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <filesystem>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


/**
 * @brief Constructor for System
 * 
 * Gets a list of the users on the system
 * Then call System::UpdateProcesses() to initialise the 
 * processes_ vector;
 */
System::System()
{
  LinuxParser::Passwd passwd;

  std::string path = LinuxParser::kPasswordPath;
  std::ifstream filestream(path);
  filestream.exceptions(std::ifstream::failbit);

  try
  {
    for(;;)
    {
      passwd = LinuxParser::Attribute<LinuxParser::Passwd>(LinuxParser::PasswdRegex, path, filestream);
      users[passwd.GID] = passwd.user;
    }
  }
  catch(const LinuxParser::EndOfFile& e)
  {
    std::cerr << e.what() << '\n';
  }

  Process::SetUsers(users);
  System::UpdateProcesses();
}

/**
 * @brief Return the CPU info
 * @return The CPU info
 */
Processor& System::Cpu() { 
  return cpu_; 
}

/**
 * @brief Add a new process to the processes_ vector
 * @param pid The PID of the process to add
 * @return (void)
 */
void System::AddNewProcess(int pid)
{
  Process newProcess {pid};
  processes_.push_back(newProcess);
}

/**
 * @brief Check if a process exists
 * 
 * If it doesn't exist, mark it as terminated. It will be removed later.
 * 
 * @param process The process to check
 * @return True if the process exists. False otherwise.
 */
bool System::ProcessExists(Process& process)
{
  if(std::filesystem::exists(LinuxParser::ProcPath(process.Pid())))
  {
    return true;
  }

  process.Terminated();

  return false;
}

/**
 * @brief Find new processes and add them to the provesses_ vector
 * @return (void)
 */
void System::FindNew()
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
}

/**
 * @brief Sort the processes
 * 
 * Sorts the processes in ascending or descending order.
 * 
 * @return (void)
 */
void System::Sort()
{
  // Sort the processes
  if (Process::Order() == Process::ASCENDING)
  {
    sort(processes_.begin(), processes_.end());
  }
  else
  {
    sort(processes_.begin(), processes_.end(), std::greater<Process>());
  }

}

/**
 * @brief Remove terminated processes from the processes_ vector
 * @return (void)
 */
void System::RemoveTerminated()
{
  for (std::vector<Process>::iterator it = processes_.begin(); it != processes_.end(); ) 
  {
    if ((*it).HasTerminated())
    {
      it = processes_.erase(it);
    }
    else
    {
      it++;
    }
  }
}

/**
 * @brief Update the process list
 * 
 * The following steps are used to update the processes
 * 
 * Get a list of the current PIDs
 * Find new processes and add them to the processes_ vector
 * Run through each of the processes and get all the required parameters
 * Sort the list of processes
 * Remove the processes that have terminated
 * 
 * @return (void)
 */
void System::UpdateProcesses()
{
  Process::ClearRunning();

  System::FindNew();

  // Run the update for each process
  for (auto& process : processes_)
  {
    if(ProcessExists(process))
    { 
      try
      {
        process.UpdateProcess(UpTime());
      }
      catch (const LinuxParser::FileNotFound& e)
      {
          // Do nothing. 
          // The process has terminated, so ignore the exception. 
          // The process will be removed from the list later.
      }
    }
  }

  Sort();

  RemoveTerminated();
}

/**
 * @brief Return a container composed of the system's processes
 * @return A container composed of the system's processes
 */
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

/**
 * @brief Return the number of processes actively running on the system
 * @return The number of processes actively running on the system
 */
int System::RunningProcesses() {

  return Process::Running();
}

/**
 * @brief Return the total number of processes on the system
 * @return The total number of processes on the system
 */
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

/**
 * @brief Process the key that has been pressed
 * 
 * Depending on the key press, set the relevant value in the process.
 * 
 * @param key The key press value
 * @return (void)
 */
void System::KeyPressed(int key)
{
  if (key == 'q')
  {
    SetDone();
  }
  else if (key == 'p')
  {
    Process::SetColumn(Process::PID);
  }
  else if (key == 'u')
  {
    Process::SetColumn(Process::USER);
  }
  else if (key == 'r')
  {
    Process::SetColumn(Process::RAM);
  }
  else if (key == 'c')
  {
    Process::SetColumn(Process::CPU);
  }
  else if (key == 'a')
  {
    Process::SetOrder(Process::ASCENDING);
  }
  else if (key == 'd')
  {
    Process::SetOrder(Process::DESCENDING);
  }
}