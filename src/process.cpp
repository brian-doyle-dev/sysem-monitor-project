#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include <ctime>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::SortColumn Process::column = Process::CPU;
Process::SortOrder Process::order = Process::DESCENDING;
std::map<int, std::string> Process::users;

int Process::running;
std::ofstream Process::myfile("log.txt", std::ios::out | std::ios::app);

Process::Process(int pid) : pid(pid) {
}

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

  float total = (cpuTime.Total() - cpuTimeTotal);
  float diff = ((processTime.utime + processTime.stime) - (this->processTime.utime + this->processTime.stime));
  float utilization = diff / (total);

  cpuTimeTotal = cpuTime.Total();
  this->processTime = processTime;

  if (processTime.running)
  {
    running++;
  }

  if (pid == 7562)
  {
    myfile << std::fixed;
    myfile << "Pid: " << pid << " : Total: " << total << " : Diff: " << diff << " : Utilization: " << utilization << "\n";
    myfile.flush();
  }

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
void Process::UpdateProcess(int systemUptime) {

  std::string path = LinuxParser::ProcPath(pid, LinuxParser::kStatusFilename);
  command = LinuxParser::Attribute<std::string>(LinuxParser::CommandNameRegex, path);
  user = LinuxParser::Attribute<int>(LinuxParser::UidRegex, path);
  utilization = UpdateCpuUtilization();
  uptime = UpdateUpTime(systemUptime);
  ram = UpdateRam();
}

void Process::Terminated()
{
  terminated = true;
}

bool Process::HasTerminated()
{
  return terminated;
}

void Process::ClearRunning()
{
  running = 0;
}

int Process::Running()
{
  return running;
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
  return command + "                                                                           "; 
}

/**
 * @brief Return this process's memory utilization
 * @return This process's memory utilization
 */
string Process::Ram() { 
  return ram + "      "; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return users[user] + "               "; 
}

long int Process::UpdateUpTime(int uptime) { 

  if ((processTime.starttime == 0) || (uptime == 0))
  {
    return 0;
  }

  int starttime = (processTime.starttime / sysconf(_SC_CLK_TCK));
  int time = uptime - starttime;
//  assert(time < 36000);
  return time; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return uptime;
}

/**
 * @brief Compare function for sorting processes
 *
 * The result depends on the current setting of 'order' and
 * the result of the actual comparison.
 *
 * @param process Process to compare
 * @return Boolean result of the compare
 */
bool Process::operator<(Process const& process) const {
  bool result = false;

  if ((column == PID) && (this->pid < process.pid))
  {
    result = true;
  }
  else if ((column == CPU) && (this->utilization < process.utilization))
  {
    return true;
  }
  else if ((column == RAM) && (this->ram < process.ram))
  {
    return true;
  }
 
  return result;
}

/**
 * @brief Compare function for sorting processes
 *
 * The result depends on the current setting of 'order' and
 * the result of the actual comparison.
 *
 * @param process Process to compare
 * @return Boolean result of the compare
 */
bool Process::operator>(Process const& process) const {
  bool result = false;

  if ((column == PID) && (this->pid > process.pid))
  {
    result = true;
  } 
  else if ((column == CPU) && (this->utilization > process.utilization))
  {
    return true;
  }
  else if ((column == RAM) && (this->ram > process.ram))
  {
    return true;
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
