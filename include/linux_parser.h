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
const std::regex CommandNameRegex("^Name:\\s+([A-Za-z0-9\\-]+)");
const std::regex PasswdRegex("^([A-Za-z0-9\\-]+):x:([0-9]+):([0-9]+)");
const std::regex UidRegex("^Uid:\\s+([0-9]+)\\s");

/**
 * @brief Exception class for file not found
 * @author 
 * @since Wed Oct 27 2021
 */
class FileNotFound : public std::exception
{
public:
  FileNotFound(std::string msg)  {
    this->msg = msg.c_str(); 
  }

  const char* what() const noexcept override {
    return msg;
  }

protected:

  const char *msg;
};

/**
 * @brief Exception class for end of file
 * @author 
 * @since Wed Oct 27 2021
 */
class EndOfFile : public std::exception
{
public:
  EndOfFile(std::string msg)  {
    this->msg = msg.c_str(); 
  }

  const char* what() const noexcept override {
    return msg;
  }

protected:

  const char *msg;
};

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
void ConvertData(std::smatch match, SysMon::Passwd& result);


/**
 * @brief Get an attribute from the Linux system
 * 
 * @param regex Description of the search parameter
 * @param path The file path for the required attribute
 * @param result The value returned by the search
 * @return The value returned by the search
 */
template <typename T> T Attribute(const std::regex& regex, const std::string& path) {
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
    throw FileNotFound(path);
  }

  return data;
}

/**
 * @brief Get an attribute from the Linux system
 * 
 * @param regex Description of the search parameter
 * @param path The file path for the required attribute
 * @param filestream The filestream to use for reading data
 * @param result The value returned by the search
 * @return The value returned by the search
 */
template <typename T> T Attribute(const std::regex& regex, const std::string& path, std::ifstream& filestream) {
  T data;

  std::string line;
  std::string key;
  std::string value;
  std::smatch match;

  if (!filestream.is_open())
  {
    filestream = std::ifstream {path};
  } 

  if (filestream.is_open())
  {
    try
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
    catch(const std::ios_base::failure& e)
    {
      throw EndOfFile(path);
    }
  }
  else
  {
    throw FileNotFound(path);
  }

  return data;
}

};  // namespace LinuxParser

#endif