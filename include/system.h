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

  int TotalProcesses();               
  int RunningProcesses();    
  void RemoveTerminated();
  void Sort();
  void FindNew();

  std::string Kernel();               
  std::string OperatingSystem();     

  void KeyPressed(int key);
  void SetDone() { done = true; };
  bool Done() { return done; };

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  float memoryUtilization = 0.0;
  long uptime = 0;
  std::map<int, std::string> users;
  bool done = false;
};

#endif