#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();

  void UpdateProcesses();
  std::vector<Process>& Processes();
  void AddNewProcess(int pid);
  bool ProcessExists(Process& process);

  void UpdateMemoryUtilization();     
  float MemoryUtilization();          

  void UpdateUpTime();
  long UpTime();  

  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  float memoryUtilization = 0.0;
  long uptime = 0;
  std::map<int, std::string> users;
};

#endif