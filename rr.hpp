#ifndef RR_HPP
#define RR_HPP

#include <vector>
#include "process.hpp"

// Simple Round Robin (no arrival times used, all start at time 0)
void runRoundRobin(std::vector<Process> &processes, int quantum);

#endif
