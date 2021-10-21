#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

bool Process::ascending = false;

void Process::UpdateProcess()
{

}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

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
