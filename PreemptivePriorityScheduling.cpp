#include "priority.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Comparison function to sort processes by arrival time, then by priority
static bool compareArrival(Process &a, Process &b) {
    if (a.arrivalTime == b.arrivalTime) {
        return a.priority < b.priority; 
    }
    return a.arrivalTime < b.arrivalTime;
}

void runPreemptivePriorityScheduling(vector<Process> &allProcesses) {
    int currentTime = 0;
    int completed = 0;
    int total = (int)allProcesses.size();
    int cpuBusyTime = 0;

    // Sort processes by arrival time (and priority as tiebreaker)
    sort(allProcesses.begin(), allProcesses.end(), compareArrival);

    vector<Process*> readyQueue;
    
    while (completed < total) {
        // Add all processes that have arrived at the current time to the ready queue
        for (int i = 0; i < total; i++) {
            if (allProcesses[i].arrivalTime == currentTime &&
                allProcesses[i].remainingTime > 0) {
                readyQueue.push_back(&allProcesses[i]);
                cout << "Time " << currentTime
                     << ": P" << allProcesses[i].pid << " arrived." << endl;
            }
        }

        // If the ready queue is empty then the CPU is idle
        if (readyQueue.empty()) {
            cout << "Time " << currentTime << ": CPU idle" << endl;
            currentTime++;
            continue;
        }

        // Sort ready queue by priority (lowest value first)
        sort(readyQueue.begin(), readyQueue.end(),
             [](Process* a, Process* b) {
                 return a->priority < b->priority;
             });

        // Select the process with the highest priority
        Process* current = readyQueue[0];

        // Set start time (if not already set)
        if (current->startTime == -1) {
            current->startTime = currentTime;
        }

        // Run the selected process for 1 time unit
        cout << "Time " << currentTime
             << ": Running P" << current->pid
             << " (Priority " << current->priority << ")" << endl;

        current->remainingTime--;
        cpuBusyTime++;
        currentTime++;

        // If the process has finished
        if (current->remainingTime == 0) {
            current->completionTime = currentTime;
            current->turnaroundTime = current->completionTime - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            current->responseTime = current->startTime - current->arrivalTime;
            completed++;
            readyQueue.erase(readyQueue.begin());
        }
    }

    // Calculate and display performance metrics
    double totalWaiting = 0.0;
    double totalTurnaround = 0.0;
    double totalResponse = 0.0;

    cout << "\nProcess Summary (Preemptive Priority):\n";
    for (int i = 0; i < total; i++) {
        Process &p = allProcesses[i];

        totalTurnaround += p.turnaroundTime;
        totalWaiting += p.waitingTime;
        totalResponse += p.responseTime;

        cout << "P" << p.pid
             << " | Arrival: " << p.arrivalTime
             << " | Burst: " << p.burstTime
             << " | Priority: " << p.priority
             << " | Start: " << p.startTime
             << " | Completion: " << p.completionTime
             << " | Waiting: " << p.waitingTime
             << " | Turnaround: " << p.turnaroundTime
             << " | Response: " << p.responseTime
             << endl;
    }

    double avgW = totalWaiting / total;
    double avgT = totalTurnaround / total;
    double avgR = totalResponse / total;

    double utilization = (cpuBusyTime * 100.0) / currentTime;
    double throughput = (double)total / currentTime;

    cout << "\nAverage Waiting Time: " << avgW << endl;
    cout << "Average Turnaround Time: " << avgT << endl;
    cout << "Average Response Time: " << avgR << endl;
    cout << "CPU Utilization: " << utilization << "%" << endl;
    cout << "Throughput: " << throughput
         << " processes per time unit" << endl;
}
