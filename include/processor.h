#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include "monitor_types.h"

class Processor {
 public:
  Processor();

  float Utilization();  
  void Model();

  // TODO: Declare any necessary private members
 private:
    SysMon::CpuTime cpuTime {0, 0, 0, 0, 0, 0, 0}; 
    std::string model;
};

#endif