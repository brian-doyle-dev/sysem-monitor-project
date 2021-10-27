#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "linux_parser.h"

class Processor {
 public:
  Processor();

  float Utilization();  
  void UpdateUtilization();
  void Model();

 private:
    LinuxParser::CpuTime cpuTime {0, 0, 0, 0, 0, 0, 0}; 
    std::string model = "";
    float utilization = 0;
    static int running;
};

#endif