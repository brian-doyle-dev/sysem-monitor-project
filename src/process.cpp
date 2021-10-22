#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

bool Process::ascending = false;


/**
 * @brief Get the CPU utilization for the process
 * 
 * Note: Used the following for CPU utilization
 *  https://man7.org/linux/man-pages/man5/proc.5.html
 *
 * 
 * @return 
 */
float Process::UpdateCpuUtilization()
{
  std::string path = LinuxParser::ProcPath(LinuxParser::kStatFilename);
  SysMon::CpuTime cpuTime = LinuxParser::Attribute<SysMon::CpuTime>(LinuxParser::UtilizationRegex, path);

  path = LinuxParser::ProcPath(pid, LinuxParser::kStatFilename);
  SysMon::CpuUtilization processTime = LinuxParser::Attribute<SysMon::CpuUtilization>(LinuxParser::StatRegex, path);

  float total = (cpuTime.Total() - processTime.total);
  float diff = ((processTime.utime + processTime.stime) - (this->processTime.utime - this->processTime.stime));
//  utilization = 100 * ((processTime.utime + processTime.stime) - (this->processTime.utime - this->processTime.stime)) / (cpuTime.Total() - processTime.total);
  float utilization = 100 * (diff) / (total);

  processTime.total = cpuTime.Total();
  this->processTime = processTime;

  return utilization;
}

std::string Process::UpdateRam()
{
  std::string ramString;

  try {
    std::string path = LinuxParser::ProcPath(pid, LinuxParser::kStatFilename);
    int ram = LinuxParser::Attribute<SysMon::Ram>(LinuxParser::RamStatRegex, path).rss;
    ramString = std::to_string(ram);
  }
  catch (std::runtime_error& e) {
    ramString = this->ram; 
  }

  return ramString;
}

/**
 * @brief Update the process
 * 
 * Read the relevant information from the file system
 * 
 * @return (void)
 */
void Process::UpdateProcess() {

  std::string path = LinuxParser::ProcPath(pid, LinuxParser::kCmdlineFilename);
  command = LinuxParser::Attribute<std::string>(LinuxParser::CommandRegex, path);
  utilization = UpdateCpuUtilization();
  ram = UpdateRam();
}

/**
 * @brief Return process id 
 * @return The process id
 */
int Process::Pid() { 
  return pid; 
}


/**
 * @brief Return CPU utilization for process
 * @return The CPU utilization for process
 */
float Process::CpuUtilization() { 
  return utilization; 
}

/**
 * @brief Return the command used to start the process
 * @return The command used to start the process
 */
string Process::Command() { 
  return command; 
}

/**
 * @brief Return this process's memory utilization
 * @return This process's memory utilization
 */
string Process::Ram() { 
  return ram; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return string(); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return 0; 
}

/**
 * @brief Compare function for sorting processes
 *
 * The result depends on the current setting of 'ascending' and
 * the result of the actual comparison.
 *
 * @param process1 First process to compare
 * @param process2 Second process to compare
 * @return Boolean result of the compare
 */
bool Process::operator<(Process const& process) const {
  bool result;

  if (this->pid <= process.pid)
  {
    result = true;
  }

  if (!ascending)
  {
    result = ! result;
  }

  return result;
}

bool Process::operator==(Process const& process) const {
  if (this->pid == process.pid)
  {
    return true;
  }

  return false;
}

/**
 * @brief Compare function for sorting processes
 *
 * The result depends on the current setting of 'ascending' and
 * the result of the actual comparison.
 *
 * @param process1 First process to compare
 * @param process2 Second process to compare
 * @return Boolean result of the compare
 */
bool Process::Compare(Process process1, Process process2)
{
  bool result;

  if (process1.pid <= process2.pid)
  {
    result = true;
  }

  if (!ascending)
  {
    result = ! result;
  }

  return result;
}
