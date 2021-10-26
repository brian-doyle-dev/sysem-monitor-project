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
const std::regex PidsRegex {"^/proc/([0-9]+)$"};
const std::regex CpuModelRegex {"^model name[\\s]+:\\s(.+)"};
const std::regex UtilizationRegex {"^cpu\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)"};
const std::regex OsRegex {"^PRETTY_NAME=\\\"(.*)\\\""};
const std::regex KernelRegex {"^Linux version ([0-9a-zA-Z\\.-]+)"};
const std::regex MemTotalRegex {"^MemTotal:\\s+([0-9]+)"};
const std::regex MemFreeRegex {"^MemFree:\\s+([0-9]+)"};
const std::regex MemAvailableRegex {"^MemAvailable:\\s+([0-9]+)"};
const std::regex UptimeRegex {"^([0-9]+)"};
const std::regex CommandRegex {"^([a-zA-Z0-9\\-\\s/]+)"};
const std::regex StatRegex("^([0-9]+)\\s(\\([a-zA-Z_\\-0-9]+\\)) ([IRSDZTtWXxKWP]) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9\\-]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9\\-]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+) ([0-9]+)");
const std::regex RamStatRegex("^[0-9]+\\s\\([a-zA-Z_\\-0-9]+\\) [IRSDZTtWXxKWP] [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9\\-]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ [0-9\\-]+ [0-9]+ [0-9]+ [0-9]+ [0-9]+ ([0-9]+)");
const std::regex StatusRegex("^Name:\\s+([A-Za-z0-9\\-]+)");


// Helper functions
std::string ProcPath(int pid);
std::string ProcPath(std::string file);
std::string ProcPath(int pid, std::string file);
void ConvertData(std::smatch match, std::string& result);
void ConvertData(std::smatch match, int& result);
void ConvertData(std::smatch match, long& result);
void ConvertData(std::smatch match, SysMon::CpuTime& result);
void ConvertData(std::smatch match, SysMon::CpuUtilization& result);
void ConvertData(std::smatch match, SysMon::Ram& result);

//template <typename T> T Attribute(const std::string& regexString, std::string path);
/**
 * @brief Get an attribute from the Linux system
 * 
 * @param regex Description of the search parameter
 * @param path The file path for the required attribute
 * @param result The value returned by the search
 * @return The value returned by the search
 */
template <typename T> T Attribute(const std::regex& regex, std::string path) {
  T data;

  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(path);
  std::smatch match;

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
  else
  {
    throw std::runtime_error("File not open: " + path);
  }

  return data;
}

};  // namespace LinuxParser

#endif