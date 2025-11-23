#ifndef MLQ_SCHEDULER_HPP
#define MLQ_SCHEDULER_HPP

#include <vector>
#include "process.hpp"

// Multi-Level Queue Scheduling:
// - Foreground queue: Round Robin
// - Background queue: FCFS
void simulateMLQScheduling(std::vector<Process>& processes, int foregroundQuantum);

#endif

