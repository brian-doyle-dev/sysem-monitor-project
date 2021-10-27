#include <cassert>
#include "processor.h"
#include "linux_parser.h"

Processor::Processor()
{
  Utilization();
  Model();
}

/**
 * @brief Return the CPU utilization
 * @return The CPU utilization
 */
float Processor::Utilization() {
  return utilization;
}

/**
 * @brief Read the CPU time data from the system and update the utilization
 * @return (void)
 */
void Processor::UpdateUtilization() {
  LinuxParser::CpuTime cpuTime;
  cpuTime = LinuxParser::Attribute<LinuxParser::CpuTime>(LinuxParser::UtilizationRegex, LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

  LinuxParser::CpuTime diff = cpuTime - this->cpuTime;
  this->cpuTime = cpuTime;

  try
  {
     utilization = (float(diff.Total() - diff.idle))/(float(diff.Total())) * 100.0;
  }
  catch(const std::exception& e)
  {
    // Do nothing
  }
}

/**
 * @brief Read the processor model from the system
 * @return (void)
 */
void Processor::Model() {
  model = LinuxParser::Attribute<std::string>(LinuxParser::CpuModelRegex, LinuxParser::kProcDirectory + LinuxParser::kCpuinfoFilename);
}
