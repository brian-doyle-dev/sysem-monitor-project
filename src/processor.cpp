#include <cassert>
#include "processor.h"
#include "linux_parser.h"

Processor::Processor()
{
  Utilization();
  Model();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  SysMon::CpuTime cpuTime;
  cpuTime = LinuxParser::Attribute<SysMon::CpuTime>(std::string(LinuxParser::UtilizationRegex), 
                                              LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

  SysMon::CpuTime diff = cpuTime - this->cpuTime;
  this->cpuTime = cpuTime;

  int total = diff.Total();
  int minus_idle = diff.Total() - diff.idle;

  float utilization = (float(minus_idle))/(float(total)) * 100.0;
  //float utilization = (float(cpuTime.Total() - cpuTime.idle) / float(cpuTime.Total())) * 100.0; 

  if (utilization <= 0.0)
  {
   assert(utilization >= 0.0);
   assert(utilization <= 100.0);
   
  }

  assert(utilization > 0.0);
  assert(utilization <= 100.0);
   
  return utilization; 
}

void Processor::Model() {
  model = LinuxParser::Attribute<std::string>(std::string(LinuxParser::CpuModelRegex), 
                                              LinuxParser::kProcDirectory + LinuxParser::kCpuinfoFilename);
}
