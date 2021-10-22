#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "monitor_types.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid(pid) {
      Process::ascending = false;
  }
  
  float UpdateCpuUtilization();
  std::string UpdateRam();
  void UpdateProcess();

  static bool Compare(Process process1, Process process2);

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& process) const;  
  bool operator==(Process const& process) const;

  static bool ascending;

 private:
  enum SortColumn {PID, USER, CPU, RAM, TIME, COMMAND};
  SortColumn sortColumn = PID;

  int pid;
  float utilization;
  std::string user;
  std::string ram;
  std::string command;
  long int uptime;

  SysMon::CpuUtilization processTime = {0, 0, 0, 0, 0, 0, 0.0};

};

#endif