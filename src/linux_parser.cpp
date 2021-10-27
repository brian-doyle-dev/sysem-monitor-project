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


/**
 * @brief Get the PIDs of all the processes running on the system
 * 
 * @return A vector containing the list of processes
 */
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
 * 
 * @param file The file name
 * @return path The path for the file
 */
std::string LinuxParser::ProcPath(std::string file)
{
  std::string path = LinuxParser::kProcDirectory + file;
  return path;
}


/**
 * @brief Calculate the path for the file and PID
 * 
 * @param pid The PID for the process
 * @param file The file name
 * @return path The path for the file
 */
std::string LinuxParser::ProcPath(int pid, std::string file)
{
  std::string path = LinuxParser::kProcDirectory + std::to_string(pid) + file;
  return path;
}

/**
 * @brief Calculate the path for the PID
 * 
 * @param pid The PID for the process
 * @return path The path for the PID
 */
std::string LinuxParser::ProcPath(int pid)
{
  std::string path = LinuxParser::kProcDirectory + std::to_string(pid);
  return path;
}

/**
 * @brief Convert the data found by Attribute to the required type <std::string>
 * 
 * @param match The regex match 
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
 * @param match The regex match 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, int& result)
{
    result = stoi(match[1].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <long>
 * 
 * @param match The regex match 
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, long& result)
{
    result = stol(match[1].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <LinuxParser::CpuTime>
 * 
 * @param match The regex match
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, CpuTime& result)
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
 * @brief Convert the data found by Attribute to the required type <LinuxParser::CpuUtilization>
 * @param The regex match
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, CpuUtilization& result)
{
    result.utime = stol(match[14].str());
    result.stime = stol(match[15].str());
    result.cutime = stol(match[16].str());
    result.cstime = stol(match[17].str());
    result.starttime = stol(match[22].str());
    if (match[3].str() == "R")
    {
      result.running = true;
    }
    else
    {
      result.running = false;
    }
}

/**
 * @brief Convert the data found by Attribute to the required type <LinuxParser::Ram>
 * @param The regex match
 * @param result The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, Ram& result)
{
    result.rss = stoi(match[1].str());
}

/**
 * @brief Convert the data found by Attribute to the required type <LinuxParser::Passwd>
 * @param The regex match
 * @param esult The converted data
 * @return (void)
 */
void LinuxParser::ConvertData(std::smatch match, Passwd& result)
{
  result.user = match[1].str();
  result.UID = stoi(match[2].str());
  result.GID = stoi(match[3].str());
}
