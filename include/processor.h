#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "monitor_types.h"

class Processor {
 public:
  Processor();

  float Utilization();  
  void UpdateUtilization();
  void Model();

 private:
    SysMon::CpuTime cpuTime {0, 0, 0, 0, 0, 0, 0}; 
    std::string model = "";
    float utilization = 0;
};

#endif