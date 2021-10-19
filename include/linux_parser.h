#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include "monitor_types.h"

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

//Regular Expressions
const std::string PidsRegex {"^/proc/([0-9]+)$"};
const std::string CpuModelRegex {"^model name[\\s]+:\\s(.+)"};
const std::string UtilizationRegex {"^cpu\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)"};


// Helper functions
/**
 * @brief CPU times from the /proc/stat file for calculating the utilization
 * @author 
 * @since Mon Oct 18 2021
 */
void ConvertData(std::smatch match, std::string& result);
void ConvertData(std::smatch match, int& result);
void ConvertData(std::smatch match, SysMon::CpuTime& result);

//template <typename T> T Attribute(const std::string& regexString, std::string path);
/**
 * @brief Get an attribute from the Linux system
 * 
 * @param regex Description of the search parameter
 * @param path The file path for the required attribute
 * @param result The value returned by the search
 * @return The value returned by the search
 */
template <typename T> T Attribute(const std::string& regexString, std::string path) {
  T data;

  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(path);
  std::smatch match;
  std::regex regex(regexString);

  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      if (std::regex_search(line, match, regex))
      {
        ConvertData(match, data);
        break;
       }
    }
  }

  return data;
}

};  // namespace LinuxParser

#endif