#ifndef MLQ_HPP
#define MLQ_HPP

#include <vector>
#include "process.hpp"

// Multi-Level Queue Scheduling:
// High-priority processes -> Round Robin
// Low-priority processes  -> FCFS
void runMLQ(std::vector<Process> &allProcesses, int timeQuantumHigh);

#endif
