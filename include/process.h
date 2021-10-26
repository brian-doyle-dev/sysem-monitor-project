#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <fstream>
#include <map>

#include "monitor_types.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {

 public:

  enum SortColumn {PID, USER, CPU, RAM, TIME, COMMAND};
  enum SortOrder {ASCENDING, DESCENDING};

  Process(int pid);  
  
  float UpdateCpuUtilization();
  std::string UpdateRam();
  long int UpdateUpTime(int uptime);
  void UpdateProcess(int uptime);
  void Terminated();
  bool HasTerminated();
  static void ClearRunning();
  static int Running();
  static bool Compare(Process process1, Process process2);

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp

  bool operator>(Process const& process) const; 
  bool operator<(Process const& process) const;  
  bool operator==(Process const& process) const;
  
  static void SetColumn(SortColumn col) { column = col; }
  static void SetOrder(SortOrder ord) { order = ord; }
  static void SetUsers(std::map<int, std::string>& usr) { users = usr; }
  
  static SortOrder Order() { return order; }  
  static SortColumn Column() { return column; } 

 private:
  static SortColumn column;
  static SortOrder order;

  int pid = 0;
  int cpuTimeTotal = 0;
  float utilization = 0.0;
  int user = 0;
  std::string ram = "";
  std::string command = "";
  long int uptime = 0;
  bool terminated = false;
  static int running;

  SysMon::CpuUtilization processTime = {0, 0, 0, 0, 0, 0, false, 0.0};
  static std::map<int, std::string> users;
  static std::ofstream myfile;
};

#endif