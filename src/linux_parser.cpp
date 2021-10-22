#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector> 
#include <filesystem>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: Updated this to use std::filesystem and std::regex
vector<int> LinuxParser::Pids() {
  vector<int> pids;

  const std::filesystem::path proc{kProcDirectory};
  const std::regex rgx(PidsRegex);
  std::smatch match;
  
  for (auto const& dir_entry : std::filesystem::directory_iterator{proc})
  {
      std::string dir{dir_entry.path().string()};
      if (std::regex_search(dir, match, rgx))
      {
          pids.push_back(stoi(match[1].str()));
      }
  }

  return pids;
}


/**
 * @brief Calculate the path for the file
 * @param pid
 * @param file
 * @return path
 */
std::string LinuxParser::ProcPath(std::string file)
{
  std::string path = LinuxParser::kProcDirectory + file;
  return path;
}


/**
 * @brief Calculate the path for the file and PID
 * @param pid
 * @param file
 * @return path
 */
std::string LinuxParser::ProcPath(int pid, std::string file)
{
  std::string path = LinuxParser::kProcDirectory + std::to_string(pid) + file;
  return path;
}


/**
 * @brief Convert the data found by Attribute to the required type <std::string>
 * 
 * @param data The input string 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, std::string& result)
{
    result = match[1].str();
}

/**
 * @brief Convert the data found by Attribute to the required type <int>
 * 
 * @param data The input string 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, int& result)
{
    result = stoi(match[1].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <int>
 * 
 * @param data The input string 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, long& result)
{
    result = stol(match[1].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <SysMon::CpuTime>
 * 
 * @param data The input string 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, SysMon::CpuTime& result)
{
  result.user = stoi(match[1].str());
  result.nice = stoi(match[2].str());
  result.system = stoi(match[3].str());
  result.idle = stoi(match[4].str());
  result.iowait = stoi(match[5].str());
  result.irq = stoi(match[6].str());
  result.softirq = stoi(match[7].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <SysMon::CpuUtilization>
 * @param match
 * @param result
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, SysMon::CpuUtilization& result)
{
    result.utime = stoi(match[14].str());
    result.stime = stoi(match[15].str());
    result.cutime = stoi(match[16].str());
    result.cstime = stoi(match[17].str());
    result.starttime = stoi(match[22].str());
}


// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
